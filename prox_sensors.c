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
    const double timeout_us
    )
{
    clock_t startTime, endTime;
    int pulseWidth = 0;
    double elapsedTime = 0;

    // Set alert on echo pin
    gpioSetAlertFuncEx(sensor->echoGpio, sonarEcho, (void *) (&pulseWidth));

    // Clear trigger pin
    gpioWrite(sensor->triggerGpio, 0);
    usleep(1000);

    // Raise trigger for 10 microseconds
    gpioWrite(sensor->triggerGpio, 1);
    gpioDelay(1000);
    gpioWrite(sensor->triggerGpio, 0);

    //Check echo pin
    while(gpioRead(sensor->echoGpio) == 0){
       startTime = clock();
    }
    printf("here\n");
    while(gpioRead(sensor->echoGpio) == 1 &&
          elapsedTime < timeout_us / 1000000.0){
       endTime = clock();
       elapsedTime = (double) (endTime-startTime) / CLOCKS_PER_SEC;
    }

    // Get distance in feet
    return elapsedTime * 1158.7/2.0;
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
