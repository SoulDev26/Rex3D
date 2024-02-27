#include "include/utils/ref.h"

#include <assert.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>


struct ref_t {
    void *value;
    size_t value_size;
    size_t use_count;
    bool separate_mem; // indicates if the memory of the reference is allocated separately or together
};

ref_t *ref_new(const void *value, size_t size) {
    ref_t *ref = malloc(sizeof(ref_t) + size);
    ref->value = ref + 1;
    ref->value_size = size;
    ref->use_count = 1;
    ref->separate_mem = false;

    if (value) {
        memcpy(ref->value, value, size);
    }

    return ref;
}

// decrease the use_count, and if it == 0, it will free the reference and its value 
void ref_free(ref_t *ref) {
    assert(ref);

    --ref->use_count;

    if (ref->use_count == 0) {
        if (ref->separate_mem) {
            free(ref->value);
        }

        free(ref);
    }
}

ref_t *ref_from(const void *mem) {
    assert(mem);

    ref_t *ref = malloc(sizeof(ref_t));
    ref->value = (void*)mem;
    ref->use_count = 1;
    ref->separate_mem = true;

    return ref;
}

// increase the use_count and returns the *ref
ref_t *ref_copy(ref_t *ref) {
    assert(ref);

    ++ref->use_count;
    return ref;
}

// Clones the reference and value, and sets use_count to 1
ref_t *ref_clone(const ref_t *ref) {
    assert(ref);

    size_t size = sizeof(ref_t);
    if (ref->separate_mem) {
        size += ref->value_size;
    }
    
    ref_t *new_ref = malloc(size);
    memcpy(new_ref, ref, size);

    if (ref->separate_mem) {
        new_ref->value = malloc(ref->value_size);
        memcpy(new_ref->value, ref->value, ref->value_size);
    } else {
        new_ref->value = new_ref + 1;
    }

    return new_ref;
}

size_t ref_use_count(const ref_t *ref) {
    assert(ref);

    return ref->use_count;
}

void *ref_value(const ref_t *ref) {
    assert(ref);

    return ref->value;
}
