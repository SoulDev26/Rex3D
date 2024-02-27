#ifndef TRON_MATH_MATH_H
#define TRON_MATH_MATH_H

#include <math.h>


inline double degrees(double radians) {
    return radians * (180.f / M_PI);
}

inline double radians(double degree) {
    return degree * (M_PI / 180.f);
}


#endif //TRON_MATH_MATH_H
