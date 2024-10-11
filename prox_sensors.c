#include "prox_sensors.c"
#include <wiringPi.h>
#include <stdio.h>
#include <sys/time.h>

struct timeval {
    time_t      tv_sec;     /* seconds */
    suseconds_t tv_usec;    /* microseconds */
};

double getPulseLength(ProximitySensor sensor);
static void getTime(struct

ProximitySensor initProxSensor(
    const int triggerGpio, 
    const int echoGpio
    )
{
    ProximitySensor sensor;
    sensor.triggerGpio = triggerGpio;
    sensor.echoGpio = echoGpio;

    pinMode(trigger, OUTPUT);
    pinMode(echo, INPUT);
    digitalWrite(trigger, LOW);
    usleep(500);

    return sensor;
}

double readSensor(
    const ProximitySensor sensor, 
    const double timeout_us
    )
{
    struct timeval start, end, measureStart, measureEnd;
    int elapsed, pulseWidth;

    digitalWrite(sensor.triggerGpio, HIGH);
    usleep(10);
    digitalWrite(sensor.triggerGpio, LOW);

    gettimeofday(&start,NULL);
    gettimeofday(&end,NULL);

    elapsed = ((end.tv_sec - start.tv_sec) * 1000000) + (end.tv_usec - start.tv_usec);

    while(digitalRead(sensor.echo) == LOW && elapsed < timeout)
    {
        gettimeofday(&measureStart, NULL);
        while ( digitalRead(echo) == HIGH );
        gettimeofday(&measureEnd,NULL);

        gettimeofday(&end,NULL);
        elapsed = ((end.tv_sec - start.tv_sec) * 1000000) + (end.tv_usec - start.tv_usec);
    }

    pulseWidth = ((measureEnd.tv_sec - measureStart.tv_sec) * 1000000) 
                  + (measureEnd.tv_usec - measureStart.tv_usec);

    // Get distance in feet
    return (pulseWidth/1000000.0) * 1158.7/2.0;
}

