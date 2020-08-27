#include "closest_brute.h"
#include <math.h>

double brute_force(struct Point P[], size_t n) {
    double min = INFINITY;
    double curr_min;
    for (int i = 0; i < n; i++){
        for (int j = i+1; j < n; j++){
            curr_min = dist(P[i], P[j]);
            if (curr_min < min){
                min = curr_min;
            }
        }
    }
    return min;
} 
