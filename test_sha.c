#include <string.h>
#include "common.h"
#include "err.h"
#include "cavs.h"
#include "tester.h"

err_t sha_init(global_t *glb, tester_t *t)
{
     t->plain = &glb->params[1];
     t->attr = T_HASH;
     return RV_OK;
}

bool sha_verify(global_t *glb, tester_t *t)
{
     if (glb->params[2].size &&
         (t->out->size == glb->params[2].size) &&
         memcmp(glb->params[2].buf, t->out->buf, glb->params[2].size) == 0)
          return true;
     return false;
}
