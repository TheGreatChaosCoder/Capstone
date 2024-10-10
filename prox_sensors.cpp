#include "prox_sensors.cpp"
#include <pigpio.h>

class ProximitySensor { 
public: 
    ProximitySensor(int gpio){
        self.gpio = gpio;
    }
    ~ProximitySensor(); // Destructor
    double readSensor(); // Read Sensor

private:
    int gpio;
}; 