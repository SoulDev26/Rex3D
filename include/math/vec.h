#ifndef TRON_MATH_VEC_H
#define TRON_MATH_VEC_H

#include "include/utils/str.h"
#include <math.h>


typedef struct {
    double x,y,z;
} vec3_t;

typedef struct {
    double x,y,z,w;
} vec4_t;

vec3_t vec3_new(double x, double y, double z);
vec3_t vec3_zero();
void vec3_add(vec3_t *vec1, const vec3_t *vec2);
void vec3_sub(vec3_t *vec1, const vec3_t *vec2);
void vec3_scal_mul(vec3_t *vec1, double scalar);
double vec3_len(const vec3_t *vec);
void vec3_norm(vec3_t *vec);
double vec3_dist(const vec3_t *vec1, const vec3_t *vec2);

void vec3_scale(vec3_t *vec, const vec3_t *scale);
void vec3_rotx(vec3_t *vec, double angle);
void vec3_roty(vec3_t *vec, double angle);
void vec3_rotz(vec3_t *vec, double angle);

str_t vec3_str(const vec3_t *vec);


vec4_t vec4_new(double x, double y, double z, double w);
vec4_t vec4_from_vec3(const vec3_t *vec);
vec4_t vec4_zero();
void vec4_add(vec4_t *vec1, const vec4_t *vec2);
void vec4_sub(vec4_t *vec1, const vec4_t *vec2);
void vec4_scal_mul(vec4_t *vec1, double scalar);
double vec4_len(const vec4_t *vec);
void vec4_norm(vec4_t *vec);

str_t vec4_str(const vec4_t *vec);


#endif //TRON_MATH_VEC_H
