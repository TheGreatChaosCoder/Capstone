#include "prox_sensors.h"
#include <pigpio.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

double getPulseLength(ProximitySensor sensor);

ProximitySensor initProxSensor(
    const int triggerGpio, 
    const int echoGpio
    )
{
    ProximitySensor sensor;
    sensor.triggerGpio = triggerGpio;
    sensor.echoGpio = echoGpio;

    gpioSetMode(triggerGpio, PI_OUTPUT);
    gpioSetMode(echoGpio, PI_INPUT);
    gpioWrite(triggerGpio, 1);
    usleep(500);

    return sensor;
}

double readSensor(
    const ProximitySensor * sensor,
    const double timeout_us
    )
{
    struct timeval start, end, measureStart, measureEnd;
    int elapsed, pulseWidth;

    gpioWrite(sensor->triggerGpio, 1);
    usleep(10);
    gpioWrite(sensor->triggerGpio, 0);

    gettimeofday(&start,NULL);
    gettimeofday(&end,NULL);

    elapsed = ((end.tv_sec - start.tv_sec) * 1000000) + (end.tv_usec - start.tv_usec);

    while(gpioRead(sensor->echoGpio) == 0 && elapsed < timeout_us)
    {
        gettimeofday(&measureStart, NULL);
        while ( gpioRead(sensor->echoGpio) == 1 );
        gettimeofday(&measureEnd,NULL);

        gettimeofday(&end,NULL);
        elapsed = ((end.tv_sec - start.tv_sec) * 1000000) + (end.tv_usec - start.tv_usec);
    }

    pulseWidth = ((measureEnd.tv_sec - measureStart.tv_sec) * 1000000) 
                  + (measureEnd.tv_usec - measureStart.tv_usec);

    // Get distance in feet
    return (pulseWidth/1000000.0) * 1158.7/2.0;
}

