#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H
 
   #ifdef __cplusplus
      extern "C"
      {
   #endif
    typedef struct {
        int pwmGpio;
    }  MotorController; 

    MotorController initMotorController(int pwmGpio);
    int setMotorSpeed(MotorController controller, float speed);
    void stopMotor(MotorController controller);

   #ifdef __cplusplus
      } // extern "C"
   #endif
 
#endif