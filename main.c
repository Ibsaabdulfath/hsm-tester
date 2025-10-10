#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "common.h"
#include "cavs.h"
#include "err.h"

parser_t parser;
global_t global;

err_t config_read_val(parser_t *p, char **buf)
{
     readtok(p);
     if (!peek(p, "="))
          ERR_RET(RV_INVALID_CONFIG);

     tok_clear(p);

     while (!p->eof && isspace(p->c))
          nextc(p);

     do {
          tok_put(p, p->c);
          nextc(p);
     } while (!p->eof && p->c != '\n');

     p->tok.buf[p->tok.pos] = '\0';

      *buf = malloc(p->tok.pos + 1);
     if (*buf == NULL)
          ERR_RET(RV_ALLOCATION_FAILED);

     strcpy(*buf, p->tok.buf);

     return RV_OK;
}

err_t config_read(global_t *glb, char *cfg)
{
     parser_t p;
     err_t r = RV_OK;

     parser_init(&p, cfg);
     nextc(&p);
     readtok(&p);

     while (!p.eof) {
          if (p.tok.type == TOK_COMM) {
               readtok(&p);
               continue;
          } else if (p.tok.type == TOK_STRING) {
               if (strcmp(p.tok.buf, "tester") == 0) {
                    r = config_read_val(&p, &glb->config.tester);
                    if (r != RV_OK) {
                         ERR_SET(r);
                         goto cleanup;
                    }
               } else if (strcmp(p.tok.buf, "pkcs11") == 0) {
                    r = config_read_val(&p, &glb->config.pkcs11);
                    if (r != RV_OK) {
                         ERR_SET(r);
                         goto cleanup;
                    }
               } else if (strcmp(p.tok.buf, "pin") == 0) {
                    r = config_read_val(&p, &glb->config.pin);
                    ERR_SET(r);
                    if (r != RV_OK) {
                         goto cleanup;
                    }
               } else {
                    r = RV_INVALID_CONFIG;
                    ERR_SET(r);
                    goto cleanup;
               }
          } else {
               r = RV_INVALID_CONFIG;
               ERR_SET(r);
               goto cleanup;
          }
          readtok(&p);
     }

     LOG("tester: %s\n", glb->config.tester);
     LOG("pkcs11: %s\n", glb->config.pkcs11);
     LOG("pin: %s\n", glb->config.pin);

cleanup:
     parser_free(&p);

     if (r == RV_OK)
          return RV_OK;

     if (glb->config.tester)
          free(glb->config.tester);

     if (glb->config.pkcs11)
          free(glb->config.pkcs11);

     if (glb->config.pin)
          free(glb->config.pin);

     return r;
}

err_t xxx(char *rsp, char *cfg)
{
     err_t r = RV_OK;
     uint16_t i;

     r = config_read(&global, cfg);
     if (r != RV_OK)
          error(r);

     parser_init(&parser, rsp);
     global.p_pid = getpid();

     r = cavs_main(&global, &parser);

     if (global.config.tester)
          free(global.config.tester);

     if (global.config.pkcs11)
          free(global.config.pkcs11);

     if (global.config.pin)
          free(global.config.pin);

     if (global.opt.g_count) {
          for (i = 0; i < global.opt.g_count; i++)
               data_free(&global.opt.g_params[i]);
          free(global.opt.g_params);
     }

     parser_free(&parser);

     if (r != RV_OK)
          error(r);

     return RV_OK;
}

int main(int argc, char *argv[])
{
     if (argc != 2) {
          printf("argument error.\n"
                 "Usage: %s <rsp file path>\n", argv[0]);
          return 1;
     }

     printf("file: %s\n", argv[1]);
     xxx(argv[1], "config");
     return 0;
}
