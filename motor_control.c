#include "motor_control.h"
#include <pigpio.h>

void setMotorController(const MotorController * controller, double speed);

/**
 * @brief Creates a MotorController data structure
 * This will also set the GPIO pins to be able to send
 * PWM signals to an H-Bridge circuit.
 * 
 * @param forwardPin - The pin to cause the motor to spin clockwise.
 * Pin must be a PWM pin (that is, ALT0 must set it to PWM).
 * @param reversePin - The pin to cause the motor to spin clockwise.
 * Pin must be a PWM pin (that is, ALT0 must set it to PWM).
 * @return MotorController data structure
 */
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

/**
 * @brief Set the speed of the motor
 * 
 * @param controller A pointer to a MotorController struct
 * @param speed A speed ranging from -1 to 1. Positive speed will
 * cause clockwise rotation.
 * @return 0 if successful, -1 if speed is invalid
 */
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

