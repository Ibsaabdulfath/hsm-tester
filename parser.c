#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "parser.h"


void tok_put(parser_t *p, char c)
{
     if (p->tok.pos >= (p->tok.max - 1)) {
          if ((p->tok.max * 2) >= TOK_THRESHOLD)
               error(RV_EXCEED_THREASHOLD);

          p->tok.max *= 2;
          p->tok.buf = realloc(p->tok.buf, p->tok.max);
          if (p->tok.buf == NULL)
               error(RV_ALLOCATION_FAILED);
     }
     if (p->tok.pos == -1)
          error(RV_INVALID_INDEX);
     p->tok.buf[p->tok.pos++] = c;
}

inline void tok_clear(parser_t *p)
{
     p->tok.pos = 0;
     p->tok.type = TOK_UNKNOWN;
}

inline bool peek(parser_t *p, const char *s)
{
     return (strcmp(p->tok.buf, s) == 0 ? true : false);
}

char nextc(parser_t *p)
{
     p->pos++;
     if (p->pos >= p->size) {
          p->eof = true;
          return 0;
     }

     p->c = p->buf[p->pos];
     if (p->c == '\n')
          p->line++;

     return p->c;
}

void readtok(parser_t *p)
{
     while (!p->eof && isspace(p->c))
          nextc(p);

     tok_clear(p);
     if (p->c == '#') {
          do {
               tok_put(p, p->c);
               nextc(p);
          } while (!p->eof && p->c != '\n');

          if (p->c == '\n')
               nextc(p); /* pass the new line */
          p->tok.type = TOK_COMM;
     } else if (p->c == '[' || p->c == ']' || p->c == '(' || p->c == ')') {
          tok_put(p, p->c);
          nextc(p);
          p->tok.type = TOK_PUNC;
     } else if (isalnum(p->c)) {
          do {
               tok_put(p, p->c);
               nextc(p);
          } while (!p->eof && isalnum(p->c));
          p->tok.type = TOK_STRING;
     } else if (p->c == '=') {
          tok_put(p, p->c);
          nextc(p);
          p->tok.type = TOK_EQUAL;
     } else {
          nextc(p);
          p->tok.type = TOK_UNKNOWN;
     }

     p->tok.buf[p->tok.pos] = '\0';

     LOG("%s\n", p->tok.buf);
}

err_t parser_init(parser_t *p, char *filename)
{
     FILE *fp;
     long size;

     memset(p, 0, sizeof(parser_t));

     fp = fopen(filename, "r");
     if (fp == NULL)
          error(RV_OPEN_FILE_ERROR);

     fseek(fp, 0, SEEK_END);
     size = ftell(fp);
     fseek(fp, 0, SEEK_SET);

     p->size = size;

     p->buf =  malloc(size);
     if (p->buf == NULL) {
          fclose(fp);
          error(RV_ALLOCATION_FAILED);
     }

     if (fread(p->buf, 1, size, fp) != size)
          error(RV_READ_FILE_ERROR);

     fclose(fp);

     p->tok.buf = malloc(TOK_INIT);
     if (p->tok.buf == NULL) {
          free(p->buf);
          error(RV_ALLOCATION_FAILED);
     }
     p->tok.max = TOK_INIT;
     p->pos = -1;
     p->line = 1;
     p->tok.type = TOK_UNKNOWN;

     return RV_OK;
}

void parser_free(parser_t *p)
{
     free(p->buf);
     free(p->tok.buf);
     memset(p, 0, sizeof(parser_t));
}
