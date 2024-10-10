#ifndef PROX_SENSORS_HPP 
#define PROX_SENSORS_HPP 
 
class ProximitySensor { 
public: 
    ProximitySensor(int gpio); // Constructor and Initializer
    ~ProximitySensor(); // Destructor
    double readSensor(); // Read Sensor
}; 
 
#endif // PROX_SENSORS_HPP