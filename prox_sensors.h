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

    ProximitySensor initProxSensor(int triggerGpio, int echoGpio);
    double readSensor(const ProximitySensor sensor, const double timeout_us);

   #ifdef __cplusplus
      } // extern "C"
   #endif
 
#endif // PROX_SENSORS_H
