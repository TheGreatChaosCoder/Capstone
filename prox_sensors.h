#ifndef PROX_SENSORS_H
#define PROX_SENSORS_H
 
   #ifdef __cplusplus
      extern "C"
      {
   #endif
    typedef struct {
        int triggerGpio;
        int echoGpio;
    }  ProximitySensor; 

    int initProxSensor(int triggerGpio, int echoGpio);
    double readSensor(ProximitySensor sensor);

   #ifdef __cplusplus
      } // extern "C"
   #endif
 
#endif // PROX_SENSORS_H