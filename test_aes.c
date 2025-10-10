#include <string.h>
#include "common.h"
#include "err.h"
#include "tester.h"

err_t aes_cbc_init(global_t *glb, tester_t *t)
{
     t->key = &glb->params[1];
     t->iv = &glb->params[2];
     t->plain = &glb->params[3];
     t->attr = T_IV | T_KEY;
     if (glb->opt.flag == OPT_O_ENCRYPT)
          t->attr |= T_ENCRYPT;
     else if (glb->opt.flag == OPT_O_DECRYPT)
          t->attr |= T_DECRYPT;

     return RV_OK;
}

bool aes_cbc_verify(global_t *glb, tester_t *t)
{
     if (glb->params[4].size &&
         (t->out->size == glb->params[4].size) &&
         memcmp(glb->params[4].buf, t->out->buf, glb->params[4].size) == 0)
          return true;
     return false;
}

err_t aes_ecb_init(global_t *glb, tester_t *t)
{
     t->key = &glb->params[1];
     t->plain = &glb->params[2];
     t->attr = T_KEY;
     if (glb->opt.flag == OPT_O_ENCRYPT)
          t->attr |= T_ENCRYPT;
     else if (glb->opt.flag == OPT_O_DECRYPT)
          t->attr |= T_DECRYPT;

     return RV_OK;
}

bool aes_ecb_verify(global_t *glb, tester_t *t)
{
     if (glb->params[3].size &&
         (t->out->size == glb->params[3].size) &&
         memcmp(glb->params[3].buf, t->out->buf, glb->params[3].size) == 0)
          return true;

     return false;
}

bool aes_mct_cbc_swap(global_t *glb, tester_t *t)
{


     return true;
}

bool aes_mct_cbc_setkey(global_t *glb, tester_t *t)
{

     return true;
}

bool aes_mct_cbc_verify(global_t *glb, tester_t *t, data_t *prekey)
{

     return true;
}

bool aes_mct_ecb_swap(tester_t *t)
{
     data_t *tmp;

     tmp = t->plain;
     t->plain = t->out;
     t->out = tmp;

     return true;
}

bool aes_mct_ecb_setkey(tester_t *t, data_t *prekey)
{
     unsigned size = t->key->size;
     unsigned i, j;
     if (size == 128/8) {
          for (i = 0; i < size; i++)
               prekey->buf[i] ^= t->plain->buf[i];
     } else if (size == 192/8) {
          for (i = 0 ; i < 8; i++)
               prekey->buf[i] ^= t->out->buf[i+8];
          for (i = 8, j = 0; i < size; i++, j++)
               prekey->buf[i] ^= t->plain->buf[j];
     } else if (size == 256/8) {
          for (i = 0; i < 16; i++)
               prekey->buf[i] ^= t->out->buf[i];
          for (i = 16, j = 0 ; i < size; i++, j++)
               prekey->buf[i] ^= t->plain->buf[j];
     }
     return true;
}

bool aes_mct_ecb_verify(global_t *glb, tester_t *t, data_t *prekey)
{
     if (glb->params[1].size &&
         prekey->size &&
         memcmp(glb->params[1].buf, prekey->buf, glb->params[1].size) != 0)
          return false;

     if (glb->params[2].size &&
         t->plain->size &&
         memcmp(glb->params[2].buf, t->plain->buf, glb->params[2].size) != 0)
          return false;
     return true;
}
