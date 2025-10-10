#include <string.h>
#include <stdlib.h>
#include "common.h"
#include "err.h"
#include "tester.h"

err_t rsa_init(global_t *glb, tester_t *t)
{
     unsigned n_size, e_size, d_size;

     n_size = glb->params[1].size;
     e_size = glb->params[4].size;
     d_size = glb->params[5].size;

     if ((n_size + e_size + d_size) > glb->params[1].max)
          data_realloc(&glb->params[1], (n_size + e_size + d_size));

     memcpy(glb->params[1].buf + n_size,
            glb->params[5].buf,
            d_size);
     memcpy(glb->params[1].buf + n_size + d_size,
            glb->params[4].buf,
            e_size);
     glb->params[1].size = n_size + e_size + d_size;

     t->key = &glb->params[1];
     t->plain = &glb->params[7];
     t->keysize = e_size;
     t->attr = T_KEY | T_ENCRYPT;

     return RV_OK;
}

bool rsa_verify(global_t *glb, tester_t *t)
{
     if (glb->params[6].size &&
         (t->out->size == glb->params[6].size) &&
         memcmp(glb->params[6].buf, t->out->buf, glb->params[6].size) == 0)
          return true;
     return false;
}

bool rsa_siggen_pkcs_init(global_t *glb, tester_t *t)
{
     unsigned n_size, e_size, d_size;
     err_t r;
     uint16_t i;
     bool verify = true;

     if (strcmp((char *) glb->params[0].buf, "SHA224") == 0)
          glb->mech = "CKM_SHA224_RSA_PKCS";
     else if (strcmp((char *) glb->params[0].buf, "SHA256") == 0)
          glb->mech = "CKM_SHA256_RSA_PKCS";
     else if (strcmp((char *) glb->params[0].buf, "SHA384") == 0)
          glb->mech = "CKM_SHA384_RSA_PKCS";
     else if (strcmp((char *) glb->params[0].buf, "SHA512") == 0)
          glb->mech = "CKM_SHA512_RSA_PKCS";

     for (i = 0; i < glb->opt.g_count; i++) {
          if (strcmp((char *) glb->opt.g_params[i].tag, "d") == 0) {
               verify = false;
               break;
          }
     }

     LOG("Verify: %s\n", (verify ? "true" : "false"));

     n_size = glb->opt.g_params[0].size;
     e_size = glb->opt.g_params[1].size;

     t->key = malloc(sizeof(data_t));
     if (t->key == NULL)
          ERR_RET(RV_ALLOCATION_FAILED);

     if (verify) {
          r = data_init(t->key, (n_size + e_size), NULL);
          if (r != RV_OK) {
               free(t->key);
               t->key = 0;
               return r;
          }
          memcpy(t->key->buf, glb->opt.g_params[0].buf, n_size);
          memcpy(t->key->buf + n_size,
                 glb->opt.g_params[1].buf,
                 e_size);

          t->sign = &glb->params[2];
          t->key->size = n_size + e_size;
          t->attr = T_KEY | T_VERIFY;

     } else {
          d_size = glb->opt.g_params[2].size;

          r = data_init(t->key, (n_size + e_size + d_size), NULL);
          if (r != RV_OK) {
               free(t->key);
               t->key = 0;
               return r;
          }

          memcpy(t->key->buf, glb->opt.g_params[0].buf, n_size);
          memcpy(t->key->buf + n_size,
                 glb->opt.g_params[2].buf,
                 d_size);
          memcpy(t->key->buf + n_size + d_size,
                 glb->opt.g_params[1].buf,
                 e_size);

          t->key->size = n_size + e_size + d_size;
          t->attr = T_KEY | T_SIGN;
     }

     t->keysize = e_size;
     t->plain = &glb->params[1];

     return RV_OK;
}

bool rsa_siggen_pkcs_verify(global_t *glb, tester_t *t)
{
     data_free(t->key);
     free(t->key);
     t->key = 0;

     if (glb->params[2].size &&
         (t->out->size == glb->params[2].size)  &&
         memcmp(glb->params[2].buf, t->out->buf, glb->params[2].size) == 0)
          return true;
     return false;
}
