#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "mechanism.h"
#include "mech_def.h"
#include "data.h"
#include "tester.h"
#include "err.h"

err_t cavs_get_param(global_t *glb, parser_t *p)
{
     unsigned i_par;
     unsigned par_count;
     err_t r;

     if (p->eof)
          return RV_OK;

     par_count = glb->param_count;
     for (i_par = 0; i_par < par_count; i_par++) {
          if (p->tok.type != TOK_STRING)
               ERR_RET(RV_INVALID_CAVS);


          if (strcasecmp(p->tok.buf, glb->params[i_par].tag) != 0) {
               ERR_RET(RV_INVALID_CAVS);
          }

          readtok(p);
          if (!peek(p, "="))
               ERR_RET(RV_INVALID_CAVS);
          readtok(p);
          if (p->tok.type != TOK_STRING)
               ERR_RET(RV_INVALID_CAVS);

          /* this check is just for the fucking rsa
             which has a error msg instead of hex string */
          if (peek(p, "FAIL")) {
               do {
                    readtok(p);
               } while (!p->eof && !peek(p, ")"));
               readtok(p);
               return RV_INVALID_RSA;
          }

          if (glb->params[i_par].max) {
               if (glb->mode->opt_list->p[i_par].type == P_TYPE_HEX) {
                    r = hex2bin(&glb->params[i_par],
                                p->tok.buf,
                                p->tok.pos);
#ifdef DEBUG
                    data_print(&glb->params[i_par]);
#endif
               } else {
                    strncpy((char *) glb->params[i_par].buf,
                            p->tok.buf,
                            glb->mode->opt_list->p[i_par].init_value);
                    r = RV_OK;
               }


               if (r != RV_OK)
                    return r;
          }

          readtok(p);
     }
     if (i_par != par_count)
          ERR_RET(RV_INVALID_CAVS);

     return RV_OK;
}

err_t cavs_call_tester(global_t *glb, parser_t *p)
{
     tester_t test;
     data_t output;
     data_t prekey;
     err_t r;
     int i;
     bool result;
     unsigned count = 0;
     unsigned line = 0;


     if (glb->mode == NULL)
          ERR_RET(RV_INVALID_ARG);

     r = data_init(&output, 4096, NULL);
     if (r != RV_OK)
          return r;

     memset(&test, 0, sizeof(tester_t));

     test.out = &output;

     if (p->eof || p->tok.type != TOK_STRING) {
          data_free(test.out);
          return RV_OK;
     }

     line = p->line;
     r = cavs_get_param(glb,p);
     if (r != RV_OK && r != RV_INVALID_RSA) {
          data_free(test.out);
          return r;
     }

     while (!p->eof && p->tok.type == TOK_STRING) {
          if (glb->is_mct) {
               glb->mode->test_init(glb, &test);
               data_copy(&prekey, test.key);
               for (i = 0; i < 1000; i++) {
                    r = tester_run(glb, &test);
                    if (r != RV_OK) {
                         data_free(test.out);
                         return r;
                    }
#ifdef DEBUG
                    data_print(test.out);
#endif
                    glb->mode->test_mct_swap(&test);
               }
               glb->mode->test_mct_setkey(&test, &prekey);
               line = p->line;
               r = cavs_get_param(glb,p);
               if (r != RV_OK) {
                    data_free(test.out);
                    data_free(&prekey);
                    ERR_RET(RV_INVALID_CAVS);
               }

               result = glb->mode->test_mct_verify(glb, &test, &prekey);
          } else {
               if (r == RV_INVALID_RSA) {
                    r = cavs_get_param(glb,p);
                    if (r == RV_INVALID_RSA)
                         continue;
                    else if (r != RV_OK) {
                         data_free(test.out);
                         return r;
                    }
               }
               glb->mode->test_init(glb, &test);

               r = tester_run(glb, &test);
               if (r != RV_OK) {
                    data_free(test.out);
                    return r;
               }
#ifdef DEBUG
               data_print(test.out);
#endif
               result = glb->mode->test_verify(glb, &test);
               line = p->line;
               r = cavs_get_param(glb,p);
          }
          printf("[%3d] ", count++);
#ifdef DEBUG
          printf("Line: %d  ", line);
#endif
          printf("Mechanism: %s  ", glb->mech);
          if (test.key)
               printf("Size: %d  ", test.key->size);
          if (glb->opt.flag == OPT_O_ENCRYPT)
               printf("Encrypt  ");
          else if (glb->opt.flag == OPT_O_DECRYPT)
               printf("Decrypt  ");

          if (result) {
               printf("PASSED\n");
          } else {
               printf("FAILED\n");
          }

#if 0
               exit(0);
#endif

     }
     data_free(test.out);

     return RV_OK;
}

err_t cavs_init_param(global_t *glb)
{
     int i_par, i_opt;
     unsigned par_count = 0;
     struct alg_param *params = 0;
     data_t *dp;
     err_t r;

     for (i_opt = 0 ; i_opt < glb->mode->opt_count; i_opt++) {
          if (glb->opt.flag == glb->mode->opt_list[i_opt].opt) {
               params = glb->mode->opt_list[i_opt].p;
               par_count = glb->mode->opt_list[i_opt].param_count;
          }

     }

     if (params == 0 || par_count == 0)
          ERR_RET(RV_INVALID_CAVS);

     glb->param_count = par_count;

     glb->params = (data_t *) malloc(par_count * sizeof(data_t));
     if (glb->params == NULL)
          ERR_RET(RV_ALLOCATION_FAILED);

     dp = glb->params;
     for (i_par = 0; i_par < par_count; i_par++) {
          if (params[i_par].init_value) {

               r = data_init(&dp[i_par],
                             params[i_par].init_value,
                             params[i_par].name);

               if (r != RV_OK) {
                    for (i_par = i_par - 1; i_par >= 0; i_par--)
                         data_free(&dp[i_par]);
                    return r;
               }
          } else {
               r = data_init(&dp[i_par], 0, params[i_par].name);
               if (r != RV_OK) {
                    for (i_par = i_par - 1; i_par >= 0; i_par--)
                         data_free(&dp[i_par]);
                    return  r;
               }
          }
     }
     return RV_OK;
}

err_t cavs_parse_opt(global_t *glb, parser_t *p)
{
     struct alg_opt *opt;
     unsigned i_opt;
     err_t r;
     char tag[TAG_MAX];
     uint16_t s;
     bool found;
     bool no_more = false;

     while (!p->eof) {
          found = false;
          if (p->tok.type == TOK_COMM) {
               readtok(p);
               continue;
          } else if (p->tok.type == TOK_PUNC) {
               if (!peek(p, "["))
                    ERR_RET(RV_INVALID_CAVS);
               readtok(p);
               if (p->tok.type != TOK_STRING)
                    ERR_RET(RV_INVALID_CAVS);

               opt = glb->mode->opt_list;

               for (i_opt = 0; i_opt < glb->mode->opt_count; i_opt++) {
                    if (strcasecmp(p->tok.buf, opt[i_opt].name) == 0) {
                         if (opt[i_opt].opt & OPT_O) {
                              glb->opt.flag = opt[i_opt].opt;
                         } else if (opt[i_opt].opt == OPT_V) {
                              readtok(p);
                              if (!peek(p, "="))
                                   ERR_RET(RV_INVALID_CAVS);
                              readtok(p);
                              glb->opt.flag = OPT_V;
                              glb->opt.value = atoi(p->tok.buf);
                         }
                         readtok(p);
                         if (!peek(p, "]"))
                              ERR_RET(RV_INVALID_CAVS);
                         found = true;
                         break;
                    }
               }

               if (!found)
                    while (!peek(p, "]"))
                         readtok(p);

               readtok(p);
          } else if (p->tok.type == TOK_STRING) {
               break;
          }
     }

     while (!p->eof && !no_more) {
          no_more = true;
          if (p->tok.type == TOK_STRING) {
               opt = glb->mode->opt_list;

               for (i_opt = 0; i_opt < glb->mode->opt_count; i_opt++) {
                    if (strcasecmp(p->tok.buf, opt[i_opt].name) != 0)
                         continue;

                    /* we ignore anything but not OPT_P */
                    if ((opt[i_opt].opt & OPT_P) == 0)
                         continue;

                    no_more = false;

                    if (glb->opt.g_params == 0) {
                         s = 0;
                         glb->opt.g_params = malloc(sizeof(data_t));
                         if (glb->opt.g_params == 0)
                              ERR_RET(RV_ALLOCATION_FAILED);
                         glb->opt.g_count = 1;
                    } else {
                         s = glb->opt.g_count;
                         glb->opt.g_params = realloc(glb->opt.g_params,
                                                      (s + 1) * sizeof(data_t));
                         if (glb->opt.g_params == 0)
                              ERR_RET(RV_ALLOCATION_FAILED);
                         glb->opt.g_count++;
                    }

                    strncpy(tag, p->tok.buf, TAG_MAX);

                    readtok(p);
                    if (!peek(p, "=")) {
                         ERR_RET(RV_INVALID_CAVS);
                    }
                    readtok(p);

                    if (p->tok.type != TOK_STRING) {
                         ERR_RET(RV_INVALID_CAVS);
                    }
                    r = data_init(&glb->opt.g_params[s], p->tok.pos, tag);
                    if (r != RV_OK) {
                         return r;
                    }

                    hex2bin(&glb->opt.g_params[s], p->tok.buf, p->tok.pos);
#ifdef DEBUG
                    data_print(&glb->opt.g_params[s]);
#endif

                    readtok(p);
               }
          } else {
               ERR_RET(RV_INVALID_CAVS);
          }
     }

     return RV_OK;
}


void cavs_free_param(global_t *glb)
{
     unsigned i;

     for (i = 0; i < glb->param_count; i++) {
          data_free(&glb->params[i]);
     }
     free(glb->params);
}


err_t cavs_pre_test(global_t *glb, parser_t *p)
{
     err_t r;
     uint16_t i;

     while (!p->eof) {
          r = cavs_parse_opt(glb, p);
          if (r != RV_OK) {
               if (glb->opt.g_params) {
                    for (i = 0; i < glb->opt.g_count; i++)
                         data_free(&glb->opt.g_params[i]);
                    free(glb->opt.g_params);
               }
               return r;
          }

          r = cavs_init_param(glb);
          if (r != RV_OK)
               return r;

          r = cavs_call_tester(glb, p);
          cavs_free_param(glb);
          for (i = 0; i < glb->opt.g_count; i++)
               data_free(&glb->opt.g_params[i]);
          free(glb->opt.g_params);
          memset(&glb->opt, 0, sizeof(opt_t));
          if (r != RV_OK)
               return r;
     }
     return RV_OK;
}

err_t cavs_main(global_t *glb, parser_t *p)
{
     unsigned i_alg, i_mod;
     err_t r = RV_OK;
     bool mech_found = false;
     struct alg_mode *modes;

     nextc(p);
     readtok(p);

     if (p->tok.type != TOK_COMM)
          ERR_RET(RV_INVALID_CAVS);


     if (strstr(p->tok.buf, "CAVS") == NULL)
          ERR_RET(RV_INVALID_CAVS);

     readtok(p);

     while (!p->eof && p->tok.type == TOK_COMM) {
          for (i_alg = 0;
               i_alg < (sizeof(alg_list) / sizeof(*alg_list));
               i_alg++) {

               if (strstr(p->tok.buf, alg_list[i_alg].name)) {
                    if (alg_list[i_alg].is_mct) {
                         glb->is_mct = true;
                    }

                    modes = alg_list[i_alg].modes;

                    while (!p->eof && p->tok.type == TOK_COMM) {
                         for (i_mod = 0;
                              i_mod < alg_list[i_alg].mode_count;
                              i_mod++) {

                              if (strstr(p->tok.buf, modes[i_mod].name)) {
                                   glb->mode = &modes[i_mod];
                                   glb->mech = modes[i_mod].mech;
                                   mech_found = true;
                                   readtok(p);
                                   r = cavs_pre_test(glb, p);
                                   if (r != RV_OK)
                                        return r;
                              }

                              if (mech_found)
                                   return RV_OK;
                         }
                         readtok(p);
                    }
                    if (!mech_found)
                         ERR_RET(RV_MECH_NOT_SUPPORTED);
               }
          }
          readtok(p);
     }

     return RV_OK;
}
