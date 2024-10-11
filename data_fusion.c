#include "data_fusion.h"
#include <math.h>

double calculateDistance(double* data, double* position, int numData);

/**
 * @brief Assuming that sensors are in a line, 
 * see if the object being detected is within range
 * from the line itself
 * 
 * @param data An array of data points from multiple sensors
 * @param position The sensor's positions on a line from a defined center
 * @param numData The amount of data in the provided arrays
 * @param threshold The maximum distance to be considered "in range"
 * @return 1 if it's in range, 0 otherwise 
 */
int inRange(
    double* data, 
    double* position, 
    int numData,
    double threshold
){
    if(threshold > calculateDistance(data, position)){
        return 1;
    }
    
    return 0;
}

double calculateDistance(
    double* data, 
    double* position,
    int numData
){
    double result = 0;
    int dataUsed = 0;
    int i;

    for(i = 0; i<numData; i++){
        // This is an invalid arguemnt
        if(data[1]/position[i]) > 1{
            continue;
        }

        result += sqrt(data[i]*data[i] - position[i]*position[i]);
        dataUsed++;
    }

    return result/dataUsed;
}