// Define GPIO pins here
#ifndef IO_CONSTANTS_H
#define IO_CONSTANTS_H

   #ifdef __cplusplus
      extern "C"
      {
   #endif

   #define LOAD_BUTTON = 17;
   #define UNLOAD_BUTTON = 27
   #define SOFT_ESTOP = 22

   #define PROX_SENSOR_TRIGGER_1 = 23
   #define PROX_SENSOR_TRIGGER_2 = 24
   #define PROX_SENSOR_ECHO_1 = 25
   #define PROX_SENSOR_ECHO_2 = 16

   // These GPIOs are the PWM pins
   #define MOTOR_CONTROLLER_F = 32
   #define MOTOR_CONTROLLER_R = 33

   // Distance Threshold
   #define DISTANCE_LOAD_THRESHOLD = 0.2
   #define DISTANCE_UNLOAD_THRESHOLD = 0.2

   // Position of Sensors (ft)
   #define POS_SENSOR_1 = 0.25
   #define POS_SENSOR_2 = -0.25

   // Sensor Read Timeout (us)
   #define TIMEOUT = 100

   // Define Character Device Names
   #define CDEV_NAME_BUTTON "Button"
   #define CDEV_NAME_ESTOP "EStop"

   #ifdef __cplusplus
      } // extern "C"
   #endif

#endif
