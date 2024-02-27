#ifndef TRON_MODEL_H
#define TRON_MODEL_H

#include "include/math/vec.h"

typedef struct {
    vec3_t *verticies;
    size_t verticies_count;
    
    size_t *edges;
    size_t edges_count;

    vec3_t rot;
    vec3_t pos;
    vec3_t scale;
} model_t;

model_t *model_new(size_t verticies_count, size_t edges_count);
void model_free(model_t *model);

model_t *model_from(const model_t *model);
model_t *model_from_raw(const vec3_t *verticies, size_t verticies_count, const size_t edges[][2], size_t edges_count);

model_t *get_scale_model(double scale);
model_t *get_move_model(vec3_t delta);



#endif //TRON_MODEL_H
