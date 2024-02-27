#include "include/model.h"
#include "include/math/vec.h"

#include <assert.h>
#include <malloc.h>
#include <string.h>



model_t *model_new(size_t verticies_count, size_t edges_count) {
    model_t *model = malloc(sizeof(model_t) + sizeof(vec3_t) * verticies_count + sizeof(size_t) * 2 * edges_count);
    model->verticies_count = verticies_count;
    model->edges_count = edges_count;
    model->verticies = ((void*)model) + sizeof(model_t);
    model->edges = ((void*)model->verticies) + sizeof(vec3_t) * verticies_count;
    model->pos = vec3_zero();
    model->rot = vec3_zero();
    
    printf("Allocated model with size = %zu\n", sizeof(model_t) + sizeof(vec3_t) * verticies_count + sizeof(size_t) * edges_count);
    printf("where:\n\tsizeof(model_t)=%zu\n\tsizeof(vec3_t)=%zu\n\tverticies_count=%zu\n\tedges_count=%zu\n\tverticies_memsize=%zu\n\tedges_memsize=%zu\n",
           sizeof(model_t),
           sizeof(vec3_t),
           verticies_count,
           edges_count,
           sizeof(vec3_t) * verticies_count,
           sizeof(size_t) * edges_count
    );
    return model;
}

void model_free(model_t *model) {
    assert(model);

    free(model);
}

model_t *model_from(const model_t *model) {
    size_t size = sizeof(model_t) + sizeof(vec3_t) * model->verticies_count + sizeof(size_t) * 2 * model->edges_count;
    model_t *new_model = malloc(size);
    memcpy(new_model, model, size);
    new_model->verticies = ((void*)new_model) + sizeof(model_t);
    new_model->edges = ((void*)new_model->verticies) + sizeof(vec3_t) * new_model->verticies_count;

    return new_model;
}

model_t *model_from_raw(const vec3_t *verticies, size_t verticies_count, const size_t edges[][2], size_t edges_count) {
    model_t *model = model_new(verticies_count, edges_count);

    memcpy(model->verticies, verticies, sizeof(vec3_t) * verticies_count);
    memcpy(model->edges, edges, sizeof(size_t) * 2 * edges_count);

    return model;
}

model_t *get_scale_model(double scale);
model_t *get_move_model(vec3_t delta);
