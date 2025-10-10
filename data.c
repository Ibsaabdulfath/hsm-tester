#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"


err_t data_init(data_t *d, unsigned max_init, char *tag)
{

     if (d == 0)
          ERR_RET(RV_INVALID_ARG);

     memset(d, 0, sizeof(data_t));

     if (max_init) {
          d->buf = malloc(max_init);
          if (d->buf == NULL)
               ERR_RET(RV_ALLOCATION_FAILED);
          d->max = max_init;
     }

     if (tag) {
          if (strlen(tag) > TAG_MAX)
               ERR_RET(RV_INVALID_TAG_SIZE);
          strncpy(d->tag, tag, TAG_MAX);
     }

     return RV_OK;
}

err_t data_realloc(data_t *d, unsigned new_size)
{
     unsigned size;

     if (new_size <= d->max)
          return RV_OK;

     size = d->max;
     while (size < new_size)
          size *= 2;

     d->buf = realloc(d->buf, size);
     if (d->buf == NULL)
          ERR_RET(RV_ALLOCATION_FAILED);

     d->max = size;

     return RV_OK;
}

void data_free(data_t *d)
{
     if (d->buf)
          free(d->buf);
     memset(d, 0, sizeof(data_t));
}

void data_print(data_t *d)
{
     int i;

     LOG("SIZE: %d, DATA: ", d->size);
     for (i = 0; i < d->size; i++)
          LOG("%02x", d->buf[i]);

     LOG("\n");
}

err_t data_copy(data_t *dst, data_t *src)
{
     err_t r;

     if (dst == 0)
          ERR_RET(RV_INVALID_ARG);

     if (dst->max == 0) {
          r = data_init(dst, src->max, src->tag);
          if (r != RV_OK)
               return r;
     } else if (dst->max < src->size) {
          r = data_realloc(dst, src->size);
          if (r != RV_OK)
               return r;
     }
     memcpy(dst->buf, src->buf, src->size);
     dst->size = src->size;

     return RV_OK;
}

err_t hex2bin(data_t *d, char *s, unsigned size)
{
     unsigned char *dst;
     err_t r;

     if (s == 0)
          ERR_RET(RV_INVALID_ARG);

     if (d == 0)
          ERR_RET(RV_INVALID_ARG);

     if (size & 1)
          ERR_RET(RV_INVALID_ARG);

     if ((size / 2) > d->max) {
          r = data_realloc(d, size / 2);
          if (r != RV_OK)
               return r;
     }

     d->size = size / 2;

     dst = d->buf;

     while (size > 0) {
          sscanf(s , "%02x", (unsigned *) dst);
          dst++;
          s += 2;
          size -= 2;
     }

     return RV_OK;
}
