#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H
 
   #ifdef __cplusplus
      extern "C"
      {
   #endif
    typedef struct {
        int forward;
	     int reverse;
        int speaker;
    }  MotorController;

    MotorController initMotorController(
      const int forwardPin, const int reversePin, const int speakerPin);
    int setMotorSpeed(MotorController * controller, float speed);
    void stopMotor(MotorController * controller);
    void setSpeaker(MotorController * controller, int on)

   #ifdef __cplusplus
      } // extern "C"
   #endif
 
#endif
