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
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "motor_control.h"
#include "prox_sensors.h"
#include "data_fusion.h"
#include "IO_Constants.h"

#define BUFFER_SIZE 5

int main(){
    // Get character devices
    int cdev_button_id, cdev_estop_id, bytes_read;
    char button_buffer[BUFFER_SIZE], estop_buffer[BUFFER_SIZE];
    char zero_buffer[1] = {0};

    if((cdev_button_id = open("/dev/" + CDEV_NAME_BUTTON, O_RDWR)) == -1) {
	    printf("Cannot open device %s\n", CDEV_NAME_BUTTON);
	    return 1;
    }

    if((cdev_estop_id = open("/dev/" + CDEV_NAME_ESTOP, O_RDONLY)) == -1) {
        printf("Cannot open device %s\n", CDEV_NAME_ESTOP);
	return 1;
    }

    // Initialize All Sensors/Controllers/GPIOs
    MotorController mController;
    ProximitySensor pSensor[2];
    int motorOn, buttonInput;
    double speed, data[2];

    double positions[] = {POS_SENSOR_1, POS_SENSOR_2};

    gpioInitialise();

    mController = initMotorController(MOTOR_CONTROLLER_F, MOTOR_CONTROLLER_R);
    pSensor[0] = initProxSensor(PROX_SENSOR_TRIGGER_1, PROX_SENSOR_ECHO_1);
    pSensor[1] = initProxSensor(PROX_SENSOR_TRIGGER_2, PROX_SENSOR_ECHO_2);

    // Main Loop
    while(1)
    {
        // Check Estop First
        bytes_read = read(cdev_estop_id, estop_buffer, sizeof(estop_buffer)); 
        if (bytes_read < 0) {
            printf("Estop Read Failed, leaving...\n");
            break;
        }

        if(estop_buffer[0] == SOFT_ESTOP){
            continue;
        }

        // Check Button Presses
        bytes_read = read(cdev_button_id, button_buffer, sizeof(button_buffer)); 
        if (bytes_read < 0) {
            printf("Estop Read Failed, leaving...\n");
            break;
        }
        
        // Write a 0 to say that the button input has been read
        buttonInput = button_buffer[0];
        write(cdev_button_id, zero_buffer, sizeof(zero_buffer)); 

        // Check if motor on
        if(motorOn){
            // See if the distance threshold is met
            data[0] = readSensor(&pSensor[0], TIMEOUT);
            data[1] = readSensor(&pSensor[1], TIMEOUT);

            // Loading
            if(speed > 1){
                motorOn = (inRange(data, positions, 2, DISTANCE_LOAD_THRESHOLD) == 1);
            }
            else{
                motorOn = (inRange(data, positions, 2, DISTANCE_UNLOAD_THRESHOLD) == 0);
            }
        }

        // If button is pressed, turn motor on
        if(buttonInput == LOAD_BUTTON){
            motorOn = true;
            speed = 1;
        }
        else if(buttonInput == UNLOAD_BUTTON){
            motorOn = true;
            speed = -1;
        }

    }

    gpioTerminate();
    return 0;
}
