#include <string.h>
#include "common.h"
#include "err.h"
#include "cavs.h"
#include "tester.h"

err_t tdes_ecb_init(global_t *glb, tester_t *t)
{
     unsigned size;
     size = glb->params[1].size;

     memcpy(glb->params[1].buf + size,
            glb->params[1].buf,
            size);
     memcpy(glb->params[1].buf + (2 * size),
            glb->params[1].buf,
            size);
     glb->params[1].size = size * 3;

     t->key = &glb->params[1];
     t->plain = &glb->params[2];
     t->attr = T_KEY;
     if (glb->opt.flag == OPT_O_ENCRYPT)
          t->attr |= T_ENCRYPT;
     else if (glb->opt.flag == OPT_O_DECRYPT)
          t->attr |= T_DECRYPT;

     return RV_OK;
}

bool tdes_ecb_verify(global_t *glb, tester_t *t)
{
     if (glb->params[3].size &&
         (t->out->size == glb->params[3].size) &&
         memcmp(glb->params[3].buf, t->out->buf, glb->params[3].size) == 0)
          return true;
     return false;
}

err_t tdes_cbc_init(global_t *glb, tester_t *t)
{
     unsigned size;
     size = glb->params[1].size;

     memcpy(glb->params[1].buf + size,
            glb->params[1].buf,
            size);
     memcpy(glb->params[1].buf + (2 * size),
            glb->params[1].buf,
            glb->params[1].size);
     glb->params[1].size = size * 3;

     t->key = &glb->params[1];
     t->iv = &glb->params[2];
     t->plain = &glb->params[3];
     t->attr = T_KEY | T_IV;
     if (glb->opt.flag == OPT_O_ENCRYPT)
          t->attr |= T_ENCRYPT;
     else if (glb->opt.flag == OPT_O_DECRYPT)
          t->attr |= T_DECRYPT;

     return RV_OK;
}

bool tdes_cbc_verify(global_t *glb, tester_t *t)
{
     if (glb->params[4].size &&
         (t->out->size == glb->params[4].size) &&
         memcmp(glb->params[4].buf, t->out->buf, glb->params[4].size) == 0)
          return true;
     return false;
}

err_t tdes_mmt_ecb_init(global_t *glb, tester_t *t)
{
     unsigned size;

     size = glb->params[1].size;

     memcpy(glb->params[1].buf + size,
            glb->params[2].buf,
            size);
     memcpy(glb->params[1].buf + (2 * size),
            glb->params[3].buf,
            size);
     glb->params[1].size = size * 3;

     t->key = &glb->params[1];
     t->plain = &glb->params[4];
     t->attr = T_KEY;
     if (glb->opt.flag == OPT_O_ENCRYPT)
          t->attr |= T_ENCRYPT;
     else if (glb->opt.flag == OPT_O_DECRYPT)
          t->attr |= T_DECRYPT;

     return RV_OK;
}

bool tdes_mmt_ecb_verify(global_t *glb, tester_t *t)
{
     if (glb->params[5].size &&
         (t->out->size == glb->params[5].size) &&
         memcmp(glb->params[5].buf, t->out->buf, glb->params[5].size) == 0)
          return true;
     return false;
}
