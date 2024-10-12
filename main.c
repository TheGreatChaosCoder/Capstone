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
#include <pthread.h>
#include <semaphore.h>

#include "motor_control.h"
#include "prox_sensors.h"
#include "data_fusion.h"
#include "IO_Constants.h"

#define BUFFER_SIZE 5
#define CDEV_NAME_BUTTON_FULL "/dev/Button"
#define CDEV_NAME_ESTOP_FULL "/dev/EStop"

sem_t estop_mutex;
sem_t button_mutex; 
int buttons[3]; // load, unload, estop

void * estopButtonThread(void * ptr){
    buttons[2] = 0;

    while(1)
    {
        sem_wait(&estop_mutex);
        buttons[2] = (gpioRead(SOFT_ESTOP) == 1);
        sem_post(&estop_mutex);

	if(buttons[2]){
	   printf("EStop Button Pressed\n");
	}

        usleep(100);
    }

    pthread_exit(NULL);
}

void * loadingButtonThread(void * ptr){
    buttons[0] = 0;
    buttons[1] = 0;

    while(1)
    {
        sem_wait(&button_mutex);
        buttons[0] = (gpioRead(LOAD_BUTTON) == 1);
        buttons[1] = (gpioRead(UNLOAD_BUTTON) == 1 && !buttons[0]);
        sem_post(&button_mutex);

	if(buttons[0]){
	   printf("Load Button Pressed\n");
	}
	else if(buttons[1]){
	   printf("Unload Button Pressed\n");
	}

        usleep(200);
    }

    pthread_exit(NULL);
}

int main(){
    // Initialize All Sensors/Controllers/GPIOs
    printf("Getting All GPIOs Ready...\n");
    MotorController mController;
    ProximitySensor pSensor[2];
    int motorOn = 0;
    int buttonInput;
    double speed, data[2];

    double positions[] = {POS_SENSOR_1, POS_SENSOR_2};

    if(gpioInitialise() == PI_INIT_FAILED){
       printf("Cannot init gpio, exiting\n");
       return 1;
    }

    gpioSetMode(SOFT_ESTOP, PI_INPUT);
    gpioSetMode(LOAD_BUTTON, PI_INPUT);
    gpioSetMode(UNLOAD_BUTTON, PI_INPUT);

    mController = initMotorController(MOTOR_CONTROLLER_F, MOTOR_CONTROLLER_R);
    pSensor[0] = initProxSensor(PROX_SENSOR_TRIGGER_1, PROX_SENSOR_ECHO_1);
    pSensor[1] = initProxSensor(PROX_SENSOR_TRIGGER_2, PROX_SENSOR_ECHO_2);

    printf("Setting Up Threads and Semaphores...\n");
    pthread_t estopThread, buttonThread;

    sem_init(&button_mutex, 0, 1);
    sem_init(&estop_mutex, 0, 1);

    pthread_create(&estopThread, NULL, &estopButtonThread, NULL);
    pthread_create(&buttonThread, NULL, &loadingButtonThread, NULL);

    printf("Starting Loop...\n");

    // Main Loop
    while(1)
    {
        // Check Estop First
        sem_wait(&estop_mutex);
        if(buttons[2] == 1){
            usleep(500);
            continue;
        }
        sem_post(&estop_mutex);

        // Check Button Presses
        sem_wait(&button_mutex);
        buttonInput = buttons[0] == 1 ? LOAD_BUTTON :
                      buttons[1] == 1 ? UNLOAD_BUTTON :
                      0; 
        sem_post(&button_mutex);

        // Check if motor on
        if(motorOn){
            // Run Motor
            setMotorSpeed(&mController, speed);

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
	else{
	    stopMotor(&mController);
	}

        // If button is pressed, turn motor on
        if(buttonInput == LOAD_BUTTON){
            motorOn = 1;
            speed = 1;
        }
        else if(buttonInput == UNLOAD_BUTTON){
            motorOn = 1;
            speed = -1;
        }
	else{
	    motorOn = 0;
	}

	usleep(100);

    }

    pthread_join(estopThread, NULL);
    pthread_join(buttonThread, NULL);
    gpioTerminate();

    return 0;
}
