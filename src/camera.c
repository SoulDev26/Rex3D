#include "include/camera.h"
#include "include/math/math.h"
#include <math.h>



// Get point respect to the camera
vec3_t camera_get_respect_point(const camera_t *camera, const vec3_t *point) {
    vec3_t res_point = vec3_zero();
    
    vec3_t sub_vec = *point;
    vec3_sub(&sub_vec, &camera->pos);
   
    vec3_t ort_cos = {
        cos(radians(camera->rot.x)),
        cos(radians(camera->rot.y)),
        cos(radians(camera->rot.z))
    };
    vec3_t ort_sin = {
        sin(radians(camera->rot.x)),
        sin(radians(camera->rot.y)),
        sin(radians(camera->rot.z))
    };

    res_point.x = ort_cos.x * (ort_sin.z*sub_vec.y + ort_cos.z*sub_vec.x) - ort_sin.y*sub_vec.z;
    res_point.y = ort_sin.x * (ort_cos.y*sub_vec.z + ort_sin.y * (ort_sin.z*sub_vec.y + ort_cos.z*sub_vec.x)) + ort_cos.x * (ort_cos.z*sub_vec.y - ort_sin.z*sub_vec.x);
    res_point.z = ort_cos.x * (ort_cos.y*sub_vec.z + ort_sin.y * (ort_sin.z*sub_vec.y + ort_cos.z*sub_vec.x)) - ort_sin.x * (ort_cos.z*sub_vec.y - ort_sin.z*sub_vec.x);

    return res_point;
}

vec3_t camera_get_project_point(const camera_t *camera, const vec3_t *point) {
    vec3_t d = camera_get_respect_point(camera, point);
    vec3_rotx(&d, camera->rot.x);
    vec3_roty(&d, camera->rot.y);
    vec3_rotz(&d, camera->rot.z);
    
    vec3_t res = vec3_zero();
    
    res.x = (camera->display_pos.z / d.z) * d.x + camera->display_pos.x;
    res.y = (camera->display_pos.z / d.z) * d.y + camera->display_pos.y;

    return res;
}
