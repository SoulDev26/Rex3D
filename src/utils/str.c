#include "include/utils/str.h"

#include <assert.h>
#include <malloc.h>
#include <string.h>


str_t str_new() {
    return (str_t){
        .str = malloc(0),
        .len = 0
    };
}

void str_free(str_t *s) {
    assert(s);

    free(s->str);
    s->str = NULL;
    s->len = 0;
}

str_t str_from_raw(const char *cstr) {
    assert(cstr);

    str_t s;
    s.len = strlen(cstr);
    s.str = malloc(s.len + 1);

    strcpy(s.str, cstr);

    return s;
}

str_t str_from(const str_t *s) {
    assert(s);

    str_t new_str = {0};
    
    if (s->str != NULL) {
        new_str.len = s->len;
        new_str.str = malloc(s->len + 1);
    }

    return new_str;
}

size_t str_len(const str_t *s) {
    assert(s);
    return s->len;
}

char *str_cstr(const str_t *s) {
    assert(s);
    return s->str;
}

char str_at(const str_t *s, size_t i) {
    assert(s);
    assert(i < s->len);

    return s->str[i];
}

void str_add_raw(str_t *s1, const char *s2) {
    assert(s1);
    assert(s2);

    size_t s2_len = strlen(s2);
    char *new_raw = malloc(s1->len + s2_len + 1);
    
    strcpy(new_raw, s1->str);
    strcpy(new_raw + s1->len, s2);

    free(s1->str);
    s1->str = new_raw;
    s1->len += s2_len;
}

void str_add(str_t *s1, const str_t *s2) {
    assert(s1);
    assert(s2);

    if (s2->len == 0) {
        return;
    }

    str_add_raw(s1, s2->str);
}
