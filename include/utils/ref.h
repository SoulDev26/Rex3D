#ifndef TRON_REF_H
#define TRON_REF_H

#include <stddef.h>
#include <string.h>

#define str_ref_new(str) ref_new(str, strlen(str) + 1)


struct ref_t;
typedef struct ref_t ref_t;


ref_t *ref_new(const void *value, size_t size);
// decrease the use_count, and if it == 0, it will free the reference and its value 
void ref_free(ref_t *ref);
ref_t *ref_from(const void *mem);

// increase the use_count and returns the *ref
ref_t *ref_copy(ref_t *ref);
// Clones the reference and value, and sets use_count to 1
ref_t *ref_clone(const ref_t *ref);

size_t ref_use_count(const ref_t *ref);
void *ref_value(const ref_t *ref);



#endif //TRON_REF_H
