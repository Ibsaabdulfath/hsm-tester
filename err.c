#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "err.h"

#ifdef DEBUG
void do_log(const char *fmt, ...)
{
     va_list v;
     va_start(v, fmt);
     vfprintf(stderr, fmt, v);
     va_end(v);
}
#endif

void handle_error(const char *file, unsigned line,  err_t e)
{
     static const char *error_texts[] = {
          "OK",
          "unknown error",
          "allocation failed",
          "invalid cavs file",
          "mechanism not supported",
          "invalid mechanism",
          "invalid argument",
          "allocation exceed the threashold",
          "cannot open file",
          "cannot read file content",
          "make tmp file error",
          "invalid key size",
          "invalid index",
          "invalid tag size",
          "invlaid config file",
          "invalid rsa" /* this one never will use, it fill no one space :) */
     };

     const unsigned int num = sizeof(error_texts) / sizeof(*error_texts);

     if (e >= num)
          e = RV_UNKNOWN;

#ifdef DEBUG
     if (g_err_state.line) {
          line = g_err_state.line;
          file = g_err_state.file;
     }

     LOG("(%s: %d) %s.\n", file, line, error_texts[e]);
#else
     fprintf(stderr, "%s.\n", error_texts[e]);
#endif
     exit(1);
}
