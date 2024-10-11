#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H
 
   #ifdef __cplusplus
      extern "C"
      {
   #endif
    typedef struct {
        int leftForward;
	int leftReverse;
	int rightForward;
	int rightReverse;
    }  MotorController;

    MotorController initMotorController(int lfPin, int lrPin, int rfPin, int rrPin);
    int setMotorSpeed(MotorController controller, float speed);
    void stopMotor(MotorController controller);

   #ifdef __cplusplus
      } // extern "C"
   #endif
 
#endif
