// Define GPIO pins here
#ifndef IO_CONSTANTS_H
#define IO_CONSTANTS_H

   #ifdef __cplusplus
      extern "C"
      {
   #endif

   #define LOAD_BUTTON = 3;
   #define UNLOAD_BUTTON = 4
   #define SOFT_ESTOP = 4

   #define PROX_SENSOR_TRIGGER_1 = 5
   #define PROX_SENSOR_TRIGGER_2 = 6
   #define PROX_SENSOR_ECHO_1 = 8
   #define PROX_SENSOR_ECHO_2 = 9

   #define MOTOR_CONTROLLER_L_F = 7
   #define MOTOR_CONTROLLER_L_R = 8
   #define MOTOR_CONTROLLER_R_F = 9
   #define MOTOR_CONTROLLER_R_R = 10

   // Define Character Device Names
   #define CDEV_NAME_BUTTON "Button"
   #define CDEV_NAME_ESTOP "EStop"

   #ifdef __cplusplus
      } // extern "C"
   #endif

#endif
