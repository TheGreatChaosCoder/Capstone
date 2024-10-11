#include "motor_control.h"
#include <wiringPi.h>

MotorController initMotorController(
    int pwmGpio
    )
{
    wiringPiSetup();

    MotorController controller;
    controller.pwmGpio = pwmGpio;

    pinMode(pwmGpio, PWM_OUTPUT);
}

int setMotorSpeed(
    MotorController controller, 
    float speed
    )
{
    if(speed < 0 || speed > 1){
        return -1;
    }

    pwmWrite(controller.pwmGpio, (int) (1023 * speed));

}

void stopMotor(
    MotorController controller
)
{
    pwmWrite(controller.pwmGpio, 0);
}

