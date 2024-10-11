#include "motor_control.h"
#include <pigpio.h>

MotorController initMotorController(
    int lfPin,
    int lrPin,
    int rfPin,
    int rrPin
    )
{
    MotorController controller;
    controller.leftForward = lfPin;
    controller.leftReverse = lrPin;
    controller.rightForward = rfPin;
    controller.rightReverse = rrPin;

    gpioSetMode(pwmGpio, PI_OUTPUT);

    return controller;
}

int setMotorSpeed(
    MotorController controller,
    float speed
    )
{
    if(speed < 0 || speed > 1){
        return -1;
    }

    gpioPWM(controller.pwmGpio, (int) (255 * speed));

    return 0;
}

void stopMotor(
    MotorController controller
)
{
    gpioPWM(controller.pwmGpio, 0);
}

