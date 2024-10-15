#include "data_fusion.h"
#include <math.h>
#include <stdio.h>

double calculateDistance(const double* data, const double* position, const int numData);

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
    if(threshold > calculateDistance(data, position, numData)){
        return 1;
    }
    return 0;
}

double calculateDistance(
    const double* data, 
    const double* position,
    const int numData
){
    double result = 0;
    int calculationsUsed = 0;
    double temp, x;
    int i, j;

    for(i = 0; i<numData; i++){
<<<<<<< HEAD

	printf("Data: %f, Pos: %f", data[i], position[i]);
=======
        for(j = 0; j<numData; j++){
            // Cannot make calculations if i==j
            if(i == j){
                continue;
            }

            // cos(theta) = (d_1^2 + x^2 -d_2^2)/(2*d_1*x)
            x = position[i]-position[j];
            temp = (data[i]*data[i] + x*x - data[j]*data[j] )/(2*data[i]*x);
            temp = temp * temp;
>>>>>>> d6014c21ce3651b29cf05c84a2003275304d34f4

             // This is an invalid arguemnt
            if(temp > 1){
                continue;
            }

            // r = d_1 * sqrt(1-temp)
            result += data[i] * sqrt(1 - temp);
            dataUsed++;
        }
    }

<<<<<<< HEAD
    printf("Distance: %f\n", result/dataUsed);
=======
    // Taking average of calculations
>>>>>>> d6014c21ce3651b29cf05c84a2003275304d34f4
    return result/dataUsed;
}
