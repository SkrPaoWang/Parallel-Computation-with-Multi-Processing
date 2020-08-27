#include "closest_helpers.h"
#include <math.h>

int verbose = 0;

int compare_x(const void* a, const void* b) {
    struct Point* u = (struct Point*)a;
    struct Point* v = (struct Point*)b;
    return u -> x - v -> x;
} 

int compare_y(const void* a, const void* b) { 
    struct Point* u = (struct Point*)a;
    struct Point* v = (struct Point*)b;
    return u -> y - v -> y;
} 

//avoid signed integer overflow
double dist(struct Point p1, struct Point p2) {
    double z = pow((p1.x - p2.x),2) + pow((p1.y - p2.y),2);
   return sqrt(z); 
} 
