#ifndef TRON_UTILS_STR_H
#define TRON_UTILS_STR_H

#include <stddef.h>


typedef struct {
    char *str;
    size_t len;
} str_t;

str_t str_new();
void str_free(str_t *s);

str_t str_from_raw(const char *cstr);
str_t str_from(const str_t *s);

size_t str_len(const str_t *s);
char *str_cstr(const str_t *s);
char str_at(const str_t *s, size_t i);

void str_add_raw(str_t *s1, const char *s2);
void str_add(str_t *s1, const str_t *s2);


#endif //TRON_UTILS_STR_H
