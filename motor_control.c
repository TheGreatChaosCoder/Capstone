#include "motor_control.h"
#include <math.h>
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
    const int forwardPin,
    const int reversePin,
    const int speakerPin
    )
{
    MotorController controller;
    controller.forward = forwardPin;
    controller.reverse = reversePin;
    controller.speaker = speakerPin;

    gpioInitialise();

    gpioSetMode(forwardPin, PI_ALT0);
    gpioSetMode(reversePin, PI_ALT0);
    gpioSetMode(speakerPin, PI_OUTPUT);

    gpioPWM(forwardPin, 0);
    gpioPWM(reversePin, 0);
    gpioWrite(speakerPin, 0);

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
    const MotorController * controller,
    float speed
    )
{
    if(speed < -1 || speed > 1){
        return -1;
    }

    setMotorController(controller, speed);

    return 0;
}

void setSpeaker(
    const MotorController * controller,
    int on
    )
{
    if(on){
        gpioWrite(controller->speaker, 1);
    }
    else{
        gpioWrite(controller->speaker, 0);
    }
}

void stopMotor(
    const MotorController * controller
    )
{
    setSpeaker(controller, 0);
    setMotorController(controller, 0);
}

void setMotorController(
    const MotorController * controller,
    const double speed
    )
{
    if(speed > 0){
        gpioPWM(controller->forward, (int) (254 * speed));
        gpioPWM(controller->reverse, 0);
    }
    else{
        gpioPWM(controller->forward, 0);
        gpioPWM(controller->reverse, (int) (-254 * speed));
    }

    setSpeaker(controller, (int) (254 * speed) != 0 ? 1 : 0);
}

