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
    #define IR_SENSOR_1 = 5
    #define IR_SENSOR_2 = 6
    #define MOTOR_CONTROLLER_PIN = 7

    // Define Character Device Names
    #define CDEV_NAME_BUTTON "Button"
    #define CDEV_NAME_ESTOP "EStop"

   #ifdef __cplusplus
      } // extern "C"
   #endif

#endif