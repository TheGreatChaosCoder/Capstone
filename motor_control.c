#include "motor_control.h"
#include <pigpio.h>

MotorController initMotorController(
    int pwmGpio
    )
{
    MotorController controller;
    controller.pwmGpio = pwmGpio;

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

