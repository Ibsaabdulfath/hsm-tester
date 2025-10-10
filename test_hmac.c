#include <string.h>
#include "common.h"
#include "err.h"
#include "tester.h"

err_t hmac_init(global_t *glb, tester_t *t)
{
     if (glb->opt.value == 20)
          glb->mech = "CKM_SHA_1_HMAC";
     else if (glb->opt.value == 28)
          glb->mech = "CKM_SHA224_HMAC";
     else if (glb->opt.value == 32)
          glb->mech = "CKM_SHA256_HMAC";
     else if (glb->opt.value == 48)
          glb->mech = "CKM_SHA384_HMAC";
     else if (glb->opt.value == 64)
          glb->mech = "CKM_SHA512_HMAC";

     t->key = &glb->params[3];
     t->plain = &glb->params[4];
     t->attr = T_SIGN | T_KEY;

     return RV_OK;
}

bool hmac_verify(global_t *glb, tester_t *t)
{
     if (glb->params[5].size &&
         (t->out->size) &&
         memcmp(glb->params[5].buf, t->out->buf, glb->params[5].size) == 0)
          return true;
     return false;
}
