#include "motor_control.h"
#include <pigpio.h>

void setMotorController(const MotorController * controller, double speed);

MotorController initMotorController(
    int forwardPin,
    int reversePin
    )
{
    MotorController controller;
    controller.forward = forwardPin;
    controller.reverse = reversePin;

    gpioInitialise();
    gpioSetMode(forwardPin, PI_ALT0);
    gpioSetMode(reversePin, PI_ALT0);

    return controller;
}

// Positive = Forward
int setMotorSpeed(
    MotorController * controller,
    float speed
    )
{
    if(speed < -1 || speed > 1){
        return -1;
    }

    setMotorController(controller, speed);

    return 0;
}

void stopMotor(
    MotorController * controller
    )
{
    setMotorController(controller, 0);
}

void setMotorController(
    const MotorController * controller,
    double speed
    )
{
    if(speed > 0){
        gpioPWM(controller->forward, (int) (255 * speed));
        gpioPWM(controller->reverse, 0);
    }
    else{
        gpioPWM(controller->forward, 0);
        gpioPWM(controller->reverse, (int) (255 * speed));
    }
}

