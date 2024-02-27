#include "include/math/vec.h"
#include "include/math/math.h"
#include "include/utils/str.h"

#include <assert.h>
#include <stdio.h>


vec3_t vec3_new(double x, double y, double z) {
    return (vec3_t){x, y, z};
}

vec3_t vec3_zero() {
    return (vec3_t){0.f, 0.f, 0.f};
}

void vec3_add(vec3_t *vec1, const vec3_t *vec2) {
    vec1->x += vec2->x;
    vec1->y += vec2->y;
    vec1->z += vec2->z;
}

void vec3_sub(vec3_t *vec1, const vec3_t *vec2) {
    vec1->x -= vec2->x;
    vec1->y -= vec2->y;
    vec1->z -= vec2->z;
}

void vec3_scal_mul(vec3_t *vec1, double scalar) {
    vec1->x *= scalar;
    vec1->y *= scalar;
    vec1->z *= scalar;
}

double vec3_len(const vec3_t *vec) {
    return sqrt(vec->x*vec->x + vec->y*vec->y + vec->z*vec->z);   
}

void vec3_norm(vec3_t *vec) {
    double len = vec3_len(vec);
    vec->x /= len;
    vec->y /= len;
    vec->z /= len;
}

double vec3_dist(const vec3_t *vec1, const vec3_t *vec2) {
    return sqrt(pow(vec2->x - vec1->x, 2) + pow(vec2->y - vec1->y, 2) + pow(vec2->z - vec1->z, 2));
}


void vec3_scale(vec3_t *vec, const vec3_t *scale) {
    vec->x *= scale->x;
    vec->y *= scale->y;
    vec->z *= scale->z;
}

void vec3_rotx(vec3_t *vec, double angle) {
    double s = sin(radians(angle));
    double c = cos(radians(angle));

    vec->y = vec->y * c + vec->z * -s;
    vec->z = vec->y * s + vec->z * c;
}

void vec3_roty(vec3_t *vec, double angle) {
    double s = sin(radians(angle));
    double c = cos(radians(angle));

    vec->x = vec->x * c + vec->z * s;
    vec->z = vec->x * -s + vec->z * c;
}

void vec3_rotz(vec3_t *vec, double angle) {
    double s = sin(radians(angle));
    double c = cos(radians(angle));

    vec->x = vec->x * c + vec->y * -s;
    vec->y = vec->x * s + vec->y * c;
}

str_t vec3_str(const vec3_t *vec) {
    assert(vec);
    
    char buf[101];
    str_t s = str_new();
   
    sprintf(buf, "(%f, %f, %f)", vec->x, vec->y, vec->z);
    str_add_raw(&s, buf);

    return s;
}



vec4_t vec4_new(double x, double y, double z, double w) {
    return (vec4_t){x, y, z, w};
}

vec4_t vec4_from_vec3(const vec3_t *vec) {
    return vec4_new(vec->x, vec->y, vec->z, 0);
}

vec4_t vec4_zero() {
    return vec4_new(0,0,0,0);
}

void vec4_add(vec4_t *vec1, const vec4_t *vec2) {
    vec1->x += vec2->x;
    vec1->y += vec2->y;
    vec1->z += vec2->z;
    vec1->w += vec2->w;
}

void vec4_sub(vec4_t *vec1, const vec4_t *vec2) {
    vec1->x -= vec2->x;
    vec1->y -= vec2->y;
    vec1->z -= vec2->z;
    vec1->w -= vec2->w;
}

void vec4_scal_mul(vec4_t *vec1, double scalar) {
    vec1->x *= scalar;
    vec1->y *= scalar;
    vec1->z *= scalar;
    vec1->w *= scalar;
}

double vec4_len(const vec4_t *vec) {
    return sqrt(vec->x*vec->x + vec->y*vec->y + vec->z*vec->z + vec->w*vec->w);   
}

void vec4_norm(vec4_t *vec) {
    double len = vec4_len(vec);
    vec->x /= len;
    vec->y /= len;
    vec->z /= len;
    vec->w /= len;
}

str_t vec4_str(const vec4_t *vec) {
    assert(vec);
   
    char buf[101];
    str_t s = str_new();

    sprintf(buf, "(%f, %f, %f, %f)", vec->x, vec->y, vec->z, vec->w);
    str_add_raw(&s, buf);
    
    return s; 
}
