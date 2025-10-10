#pragma once
#include "err.h"

#define TAG_MAX 15

typedef struct {
     unsigned char *buf;
     unsigned size;
     unsigned max;
     char tag[TAG_MAX+1];
} data_t;

err_t data_init(data_t *d, unsigned max_init, char *tag);
err_t data_realloc(data_t *d, unsigned new_size);
err_t data_copy(data_t *dst, data_t *src);
void data_free(data_t *d);
void data_print(data_t *d);
err_t hex2bin(data_t *d, char *s, unsigned size);
