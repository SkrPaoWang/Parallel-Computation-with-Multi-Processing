#include "closest_serial.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

double combine_lr(struct Point P[], size_t n, struct Point mid_point, double d){
    
    struct Point* arr = malloc(sizeof(struct Point) * n);
    int index = 0;
    for (int i = 0; i < n; i++){
        if (abs(P[i].x - mid_point.x) < d){
            arr[index] = P[i];
            index += 1;
        }
    }
    qsort(arr, index, sizeof(struct Point), compare_y);
    double curr_min = d;
    for (int i = 0; i < index; i++){
        for (int j = i+1; j < index && (arr[j].y - arr[i].y) < curr_min; j++){
                if (dist(arr[i],arr[j]) < curr_min){
                    curr_min = dist(arr[i],arr[j]);
                }
        }
    }
   
    free(arr);
    
    return curr_min;

   
}

double _closest_serial(struct Point P[], size_t n){
    if ( n <= 3){
        return brute_force(P, n);
    }
    int leftsize = floor(n / 2);
    struct Point mid_point = P[leftsize];
    int rightsize = n - leftsize;
    double dl = _closest_serial(P, leftsize);
    double dr = _closest_serial(P+leftsize, rightsize);
    double d = fmin(dl, dr);
    return combine_lr(P, n, mid_point, d);

}

double closest_serial(struct Point P[], size_t n) {
    qsort(P, n, sizeof(struct Point), compare_x);
    return _closest_serial(P, n);

}
