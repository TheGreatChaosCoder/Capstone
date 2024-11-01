#include "prox_sensors.h"
#include <pigpio.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

double getPulseLength(ProximitySensor sensor);
void sonarEcho(int gpio, int level, uint32_t tick, void * diffTick);

ProximitySensor initProxSensor(
    const int triggerGpio,
    const int echoGpio
    )
{
    ProximitySensor sensor;
    sensor.triggerGpio = triggerGpio;
    sensor.echoGpio = echoGpio;

    gpioInitialise();
    gpioSetMode(triggerGpio, PI_OUTPUT);
    gpioSetMode(echoGpio, PI_INPUT);
    gpioWrite(triggerGpio, 0);
    usleep(500);

    return sensor;
}

double readSensor(
    const ProximitySensor * sensor,
    const double timeout_ms
    )
{
    struct timespec startTime, endTime;
   // int pulseWidth = 0;
    long double elapsedTime = 0;
    int error;

    // Set alert on echo pin
   // gpioSetAlertFuncEx(sensor->echoGpio, sonarEcho, (void *) (&pulseWidth));

    // Clear trigger pin
    if((error = gpioWrite(sensor->triggerGpio, 0)) != 0){
        printf("cannot write to trigger, got error %i, returning -1\n", error);
        return -1;
    }
    gpioDelay(5);

    // Raise trigger for 10 microseconds
    gpioWrite(sensor->triggerGpio, 1);
    gpioDelay(10);
    gpioWrite(sensor->triggerGpio, 0);

    //Check echo pin
    while(gpioRead(sensor->echoGpio) == 0){
       clock_gettime(CLOCK_MONOTONIC, &startTime);
    }

    while(gpioRead(sensor->echoGpio) == 1 &&
          elapsedTime < timeout_ms / 1.0E3){
       clock_gettime(CLOCK_MONOTONIC, &endTime);
       elapsedTime = (endTime.tv_sec + endTime.tv_nsec/1.0E9
                    -(startTime.tv_sec + startTime.tv_nsec/1.0E9));
    }

    // Get distance in feet
    printf("Time: %Lf, Measured distance = %Lf\n", elapsedTime*1000.0, 12*elapsedTime*1125.3/2.0 );
    return elapsedTime * 12.0 * 1125.3/2.0;
}

void sonarEcho(
    int gpio,
    int level,
    uint32_t tick,
    void * diffTick
)
{
    static uint32_t startTick, firstTick = 0;

    if( !firstTick ) firstTick = tick;

    printf("Level: %i\n", level);
    if(level == 1){
        startTick = tick;
    }
    else if(level == 0){
        *((int *)diffTick) = tick - startTick;
        printf("Diff tick %i\n", *((int *)diffTick));
    }
}
