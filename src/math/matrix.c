#include "include/math/matrix.h"
#include "include/math/math.h"
#include "include/utils/str.h"

#include <malloc.h>
#include <assert.h>
#include <math.h>


matrix_t *SCALE_MATRIX = NULL;
matrix_t *MOVE_MATRIX = NULL;
matrix_t *X_ROTATE_MATRIX = NULL;
matrix_t *Y_ROTATE_MATRIX = NULL;
matrix_t *Z_ROTATE_MATRIX = NULL;
matrix_t *CLIP_MATRIX = NULL;

void matrix_init() {
    SCALE_MATRIX = matrix_new_unit(4, 4);
    MOVE_MATRIX = matrix_new_unit(4, 4);
    X_ROTATE_MATRIX = matrix_new_unit(4, 4);
    Y_ROTATE_MATRIX = matrix_new_unit(4, 4);
    Z_ROTATE_MATRIX = matrix_new_unit(4, 4);

    CLIP_MATRIX = matrix_new_unit(4, 4);
    matrix_set(CLIP_MATRIX, 3, 3, 0);
    matrix_set(CLIP_MATRIX, 2, 3, 1);
}

const matrix_t *get_scale_matrix(const vec3_t *scale) {
    matrix_set(SCALE_MATRIX, 0, 0, scale->x);
    matrix_set(SCALE_MATRIX, 1, 1, scale->y);
    matrix_set(SCALE_MATRIX, 2, 2, scale->z);

    return SCALE_MATRIX;
}

const matrix_t *get_move_matrix(const vec3_t *delta) {
    matrix_set(MOVE_MATRIX, 0, 3, delta->x);
    matrix_set(MOVE_MATRIX, 1, 3, delta->y);
    matrix_set(MOVE_MATRIX, 2, 3, delta->z);

    return MOVE_MATRIX;
}

const matrix_t *get_xrotate_matrix(double angle) {
    double angle_cos = cos(radians(angle));
    double angle_sin = sin(radians(angle));
    
    matrix_set(X_ROTATE_MATRIX, 1, 1, angle_cos);
    matrix_set(X_ROTATE_MATRIX, 2, 1, angle_sin);
    matrix_set(X_ROTATE_MATRIX, 1, 2, -angle_sin);
    matrix_set(X_ROTATE_MATRIX, 2, 2, angle_cos);

    return X_ROTATE_MATRIX;
}

const matrix_t *get_yrotate_matrix(double angle) {
    double angle_cos = cos(radians(angle));
    double angle_sin = sin(radians(angle));
    
    matrix_set(Y_ROTATE_MATRIX, 0, 0, angle_cos);
    matrix_set(Y_ROTATE_MATRIX, 2, 0, -angle_sin);
    matrix_set(Y_ROTATE_MATRIX, 0, 2, angle_sin);
    matrix_set(Y_ROTATE_MATRIX, 2, 2, angle_cos);

    return Y_ROTATE_MATRIX;
}

const matrix_t *get_zrotate_matrix(double angle) {
    double angle_cos = cos(radians(angle));
    double angle_sin = sin(radians(angle));
    
    matrix_set(Z_ROTATE_MATRIX, 0, 0, angle_cos);
    matrix_set(Z_ROTATE_MATRIX, 1, 0, angle_sin);
    matrix_set(Z_ROTATE_MATRIX, 0, 1, -angle_sin);
    matrix_set(Z_ROTATE_MATRIX, 1, 1, angle_cos);

    return Z_ROTATE_MATRIX;
}

const matrix_t *get_clip_matrix(double fov, double aspectRatio, double far, double near) {
    matrix_set(CLIP_MATRIX, 0, 0, fov * aspectRatio);
    matrix_set(CLIP_MATRIX, 1, 1, fov);
    matrix_set(CLIP_MATRIX, 2, 2, (far+near) / (far-near));
    matrix_set(CLIP_MATRIX, 3, 2, (2*near*far) / (near-far));

    return CLIP_MATRIX;
}


matrix_t *matrix_new(size_t y_size, size_t x_size) {
    matrix_t *mat = calloc(1, sizeof(matrix_t) + sizeof(double) * x_size * y_size);
    printf("allocated memory for matrix\n");
    mat->y_size = y_size;
    mat->x_size = x_size; 
    mat->mat = ((void*)mat) + sizeof(matrix_t); //calloc(sizeof(double), x_size * y_size);

    return mat;
}

matrix_t *matrix_new_unit(size_t y_size, size_t x_size) {
    assert(y_size == x_size);

    matrix_t *mat = matrix_new(y_size, x_size);

    for (size_t i = 0; i < x_size; ++i) {
        matrix_set(mat, i, i, 1);
    }

    return mat;
}

void matrix_free(matrix_t *mat) {
    assert(mat);

    //mat->x_size = 0;
    //mat->y_size = 0;
    free(mat); //free(mat->mat);
}


matrix_t *matrix_from(const matrix_t *mat) {
    assert(mat);
    
    matrix_t *new_mat = matrix_new(mat->y_size, mat->x_size);
    matrix_copy_from_raw(new_mat, (double*)mat->mat);

    return new_mat;
}

matrix_t *matrix_from_raw(double *arr, size_t y_size, size_t x_size) {
    assert(arr);

    matrix_t *mat = matrix_new(y_size, x_size);
    matrix_copy_from_raw(mat, arr);

    return mat;
}

matrix_t *matrix_from_raw_vec3(const vec3_t *vec) {
    assert(vec);

    matrix_t *mat = matrix_new(1, 3);
    mat->mat[0] = vec->x;
    mat->mat[1] = vec->y;
    mat->mat[2] = vec->z;

    return mat;
}

matrix_t *matrix_from_column_vec3(const vec3_t *vec) {
    assert(vec);

    matrix_t *mat = matrix_new(3, 1);
    mat->mat[0] = vec->x;
    mat->mat[1] = vec->y;
    mat->mat[2] = vec->z;

    return mat; 
}

matrix_t *matrix_from_raw_vec4(const vec4_t *vec) {
    assert(vec);

    matrix_t *mat = matrix_new(1, 4);
    mat->mat[0] = vec->x;
    mat->mat[1] = vec->y;
    mat->mat[2] = vec->z;
    mat->mat[3] = vec->w;
    
    return mat;
}

matrix_t *matrix_from_column_vec4(const vec4_t *vec) {
    assert(vec);

    matrix_t *mat = matrix_new(4, 1);
    mat->mat[0] = vec->x;
    mat->mat[1] = vec->y;
    mat->mat[2] = vec->z;
    mat->mat[3] = vec->w;

    return mat; 
}


void matrix_copy_from_raw(matrix_t *dst, double *src) {
    assert(dst && src);

    for (size_t y = 0; y < dst->y_size; ++y) {
        for (size_t x = 0; x < dst->x_size; ++x) {
            matrix_set(dst, y, x, src[y * dst->x_size + x]); 
        }
    }
}


void matrix_set_raw_vec3(matrix_t *mat, const vec3_t *vec) {
    assert(mat && vec);
    assert(mat->y_size == 1 && mat->x_size == 3);
    
    matrix_set(mat, 0, 0, vec->x);
    matrix_set(mat, 0, 1, vec->y);
    matrix_set(mat, 0, 2, vec->z);
}

void matrix_set_column_vec3(matrix_t *mat, const vec3_t *vec) {
    assert(mat && vec);
    assert(mat->y_size == 3 && mat->x_size == 1);

    matrix_set(mat, 0, 0, vec->x);
    matrix_set(mat, 1, 0, vec->y);
    matrix_set(mat, 2, 0, vec->z);
}

void matrix_set_raw_vec4(matrix_t *mat, const vec4_t *vec) {
    assert(mat && vec);
    assert(mat->y_size == 1 && mat->x_size == 4);
    
    matrix_set(mat, 0, 0, vec->x);
    matrix_set(mat, 0, 1, vec->y);
    matrix_set(mat, 0, 2, vec->z);
    matrix_set(mat, 0, 3, vec->w);
}

void matrix_set_column_vec4(matrix_t *mat, const vec4_t *vec) {
    assert(mat && vec);
    assert(mat->y_size == 4 && mat->x_size == 1);

    matrix_set(mat, 0, 0, vec->x);
    matrix_set(mat, 1, 0, vec->y);
    matrix_set(mat, 2, 0, vec->z);
    matrix_set(mat, 3, 0, vec->w);
}


double matrix_get(const matrix_t *mat, size_t y, size_t x) {
    assert(mat);
    assert(y < mat->y_size);
    assert(x < mat->x_size);

    return *matrix_get_ptr(mat, y, x);
}

double *matrix_get_ptr(const matrix_t *mat, size_t y, size_t x) {
    assert(mat);
    assert(y < mat->y_size);
    assert(x < mat->x_size);

    return &mat->mat[y * mat->x_size + x]; 
}

void matrix_set(matrix_t *mat, size_t y, size_t x, double value) {
    assert(mat);
    assert(y < mat->y_size);
    assert(x < mat->x_size);

    *matrix_get_ptr(mat, y, x) = value;
}

void matrix_mul(matrix_t *res_mat, const matrix_t *mat1, const matrix_t *mat2) {
    assert(res_mat && mat1 && mat2);
    assert(res_mat->y_size == mat1->y_size && res_mat->x_size == mat2->x_size);
    assert(mat1->x_size == mat2->y_size);

    //matrix_t *res = matrix_new(mat1->y_size, mat2->x_size);
    
    for (size_t y = 0; y < mat1->y_size; ++y) {
        for (size_t x2 = 0; x2 < mat2->x_size; ++x2) {
            double sum = 0;

            for (size_t x = 0; x < mat1->x_size; ++x) {
                //sum += mat1->mat[y][x] * mat2->mat[x][x2];
                sum += matrix_get(mat1, y, x) * matrix_get(mat2, x, x2);
            }

            //res.mat[y][x2] = sum;
            //*matrix_at(&res, y, x2) = sum;
            matrix_set(res_mat, y, x2, sum);
        }
    }

    //return res;
}


str_t matrix_str(const matrix_t *mat) {
    assert(mat);
    
    char buf[101];
    str_t s = str_new();

    for (size_t y = 0; y < mat->y_size; ++y) {
        for (size_t x = 0; x < mat->x_size; ++x) {
            sprintf(buf, "%f, ", matrix_get(mat, y, x));
            str_add_raw(&s, buf);
        }
        str_add_raw(&s, "\n");
    }

    return s;
}
