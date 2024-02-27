#ifndef TRON_MATH_MATRIX_H
#define TRON_MATH_MATRIX_H

#include "include/math/vec.h"
#include "include/utils/str.h"
#include <stddef.h>


typedef struct {
    size_t x_size;
    size_t y_size;
    double *mat;
} matrix_t;


void matrix_init();
const matrix_t *get_scale_matrix(const vec3_t *scale);
const matrix_t *get_move_matrix(const vec3_t *delta);
const matrix_t *get_xrotate_matrix(double angle);
const matrix_t *get_yrotate_matrix(double angle);
const matrix_t *get_zrotate_matrix(double angle);
const matrix_t *get_clip_matrix(double fov, double aspectRatio, double far, double near);

matrix_t *matrix_new(size_t y_size, size_t x_size);
matrix_t *matrix_new_unit(size_t y_size, size_t x_size);
void matrix_free(matrix_t *mat);

matrix_t *matrix_from(const matrix_t *mat);
matrix_t *matrix_from_raw(double *arr, size_t y_size, size_t x_size);
matrix_t *matrix_from_raw_vec3(const vec3_t *vec);
matrix_t *matrix_from_column_vec3(const vec3_t *vec);
matrix_t *matrix_from_raw_vec4(const vec4_t *vec);
matrix_t *matrix_from_column_vec4(const vec4_t *vec);

void matrix_copy_from(matrix_t *dst, const matrix_t *src);
void matrix_copy_from_raw(matrix_t *dst, double *src);

void matrix_set_raw_vec3(matrix_t *mat, const vec3_t *vec);
void matrix_set_column_vec3(matrix_t *mat, const vec3_t *vec);
void matrix_set_raw_vec4(matrix_t *mat, const vec4_t *vec);
void matrix_set_column_vec4(matrix_t *mat, const vec4_t *vec);

double matrix_get(const matrix_t *mat, size_t y, size_t x);
double *matrix_get_ptr(const matrix_t *mat, size_t y, size_t x);
void matrix_set(matrix_t *mat, size_t y, size_t x, double value);

void matrix_mul(matrix_t *res_mat, const matrix_t *mat1, const matrix_t *mat2);

str_t matrix_str(const matrix_t *mat);


#endif //TRON_MATH_MATRIX_H
