#pragma once
#include <stdbool.h>
#include "err.h"

#define TOK_INIT  (512)
#define TOK_THRESHOLD (1024 * 64)

typedef enum {
     TOK_UNKNOWN,
     TOK_COMM,
     TOK_STRING,
     TOK_PUNC,
     TOK_EQUAL
} tok_type;

typedef struct {
     char *buf;
     unsigned max;
     unsigned pos;
     tok_type type;
} tok_t;

typedef struct {
     tok_t tok;

     char *buf;
     unsigned size;
     unsigned pos;
     unsigned line;
     int eof;

     char c;
} parser_t;

err_t parser_init(parser_t *p, char *filename);
void parser_free(parser_t *p);
bool peek(parser_t *p, const char *s);
void readtok(parser_t *p);
char nextc(parser_t *p);
void tok_put(parser_t *p, char c);
void tok_clear(parser_t *p);
