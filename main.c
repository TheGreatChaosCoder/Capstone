/**
 * @file main.c
 * @author Connor Johnson
 * @brief The main file that will be ran at the start.
 * @version 0.1
 * @date 2024-09-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <pigpio.h>
#include <stdio.h>

#include "motor_control.h"
#include "prox_sensors.h"
#include "data_fusion.h"
#include "IO_Constants.h"

#define BUFFER_SIZE 5

int main(){
    // Get character devices
    int cdev_button_id, cdev_estop_id, bytes_read;
    char button_buffer[BUFFER_SIZE], estop_buffer[BUFFER_SIZE];

    if((cdev_button_id = open("/dev/" + CDEV_NAME_BUTTON)) == -1) {
	printf("Cannot open device %s\n", CDEV_NAME_BUTTON);
	return 1;
    }

    if((cdev_estop_id = open("/dev/" + CDEV_NAME_ESTOP)) == -1) {
        printf("Cannot open device %s\n", CDEV_NAME_ESTOP);
	return 1;
    }

    // Initialize All Sensors/Controllers/GPIOs
    MotorController mController;
    ProximitySensor pSensor[2];

    gpioInitialise();

    mController = initMotorController(MOTOR_CONTROLLER_PIN, MOTOR_CONTROLLER_PWM_PIN);

    gpioTerminate();
    return 0;
}
