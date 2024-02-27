#ifndef TRON_CAMERA_H
#define TRON_CAMERA_H

#include "include/math/vec.h"


typedef struct {
    vec3_t pos;
    vec3_t rot;
    vec3_t display_pos;
} camera_t;

// Get point respect to the camera
vec3_t camera_get_respect_point(const camera_t *camera, const vec3_t *point);
vec3_t camera_get_project_point(const camera_t *camera, const vec3_t *point);


#endif //TRON_CAMERA_H
