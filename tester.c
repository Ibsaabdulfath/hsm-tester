#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fcntl.h>
#include <errno.h>
#include "tester.h"

err_t tester_run(global_t *glb, tester_t *t)
{
     int status;
     char s_key_size[8];
     char s_plain_size[8];
     char s_iv_size[8];
     char s_sign_size[8];
     FILE *fin = 0, *fout = 0, *fkey = 0, *fiv = 0, *fsign = 0;
     char *args[32] = {0};
     unsigned out_size;
     unsigned argc;
     pid_t pid;
     static char *key = "/tmp/iw_key";
     static char *input = "/tmp/iw_input";
     static char *output = "/tmp/iw_output";
     static char *iv = "/tmp/iw_iv";
     static char *sign = "/tmp/iw_sign";

     LOG("Tester Run\n");

     fin = fopen(input, "wb");
     if (fin == NULL)
          ERR_RET(RV_OPEN_FILE_ERROR);
     fwrite(t->plain->buf, 1, t->plain->size, fin);
     fclose(fin);

     if (t->attr & T_KEY) {
          fkey = fopen(key, "wb");
          if (fkey == NULL)
               ERR_RET(RV_OPEN_FILE_ERROR);

          fwrite(t->key->buf, 1, t->key->size, fkey);
          fclose(fkey);
     }

     if (t->attr & T_IV) {
          fiv = fopen(iv, "wb");
          if (fiv == NULL)
               ERR_RET(RV_OPEN_FILE_ERROR);

          fwrite(t->iv->buf, 1, t->iv->size, fiv);
          fclose(fiv);
     }

     if (t->attr & T_VERIFY) {
          fsign = fopen(sign, "wb");
          if (fsign == NULL)
               ERR_RET(RV_OPEN_FILE_ERROR);
          fwrite(t->sign->buf, 1, t->sign->size, fsign);
          fclose(fsign);
     };

     if (t->attr & T_KEY) {
          if (t->keysize)
               snprintf(s_key_size, 7, "%d", t->keysize);
          else
               snprintf(s_key_size, 7, "%d", t->key->size);
     }
     snprintf(s_plain_size, 7, "%d", t->plain->size);
     if (t->attr & T_IV)
          snprintf(s_iv_size, 7, "%d", t->iv->size);
     if (t->attr & T_VERIFY)
          snprintf(s_sign_size, 7, "%d", t->sign->size);

     argc = 0;
     args[argc++] = glb->config.tester;
     args[argc++] = "-m";
     args[argc++] = glb->config.pkcs11;
     args[argc++] = "-P";
     args[argc++] = glb->config.pin;
     if (t->attr & T_ENCRYPT)
          args[argc++] = "-e";
     else if (t->attr & T_DECRYPT)
          args[argc++] = "-d";
     else if (t->attr & T_HASH)
          args[argc++] = "-h";
     else if (t->attr & T_SIGN)
          args[argc++] = "-s";
     else if (t->attr & T_VERIFY) {
          args[argc++] = "-v";
          args[argc++] = "-b";
          args[argc++] = sign;
          args[argc++] = "-j";
          args[argc++] = s_sign_size;
     }

     args[argc++] = "-L";
     args[argc++] = s_plain_size;
     args[argc++] = "-f";
     args[argc++] = input;
     args[argc++] = "-o";
     args[argc++] = output;
     args[argc++] = "-a";
     args[argc++] = glb->mech;
     if (t->attr & T_KEY) {
          args[argc++] = "-l";
          args[argc++] = s_key_size;
          args[argc++] = "-k";
          args[argc++] = key;
     }
     if (t->attr & T_IV) {
          args[argc++] = "-i";
          args[argc++] = iv;
          args[argc++] = "-p";
          args[argc++] = s_iv_size;
     }
     args[argc] = NULL;

     pid = vfork();
     if (pid == 0) {
          execv(glb->config.tester, args);
          exit(0);
     }

     if (t->attr & T_KEY)
          LOG("key size: %s\n", s_key_size);
     LOG("plain size: %s\n", s_plain_size);
     if (t->attr & T_IV)
          LOG("iv size: %s\n", s_iv_size);
     if (t->attr & T_VERIFY)
          LOG("sign size: %s\n", s_sign_size);

     LOG("WAITING...\n");

     while (1) {
          pid = wait(&status);
          if (pid == -1)
               if (errno == ECHILD)
                    break;
     }

     LOG("status: %d, ret: %d\n", status, WEXITSTATUS(status));

     fout = fopen(output, "rb");
     if (fout == NULL) {
          ERR_RET(RV_OPEN_FILE_ERROR);
     }


     fseek(fout, 0, SEEK_END);
     out_size = ftell(fout);
     fseek(fout, 0, SEEK_SET);
     if (out_size >= t->out->max)
          data_realloc(t->out, out_size);

     fread(t->out->buf, 1, out_size, fout);
     fclose(fout);
     t->out->size = out_size;
#if 1
     remove(input);
     remove(output);
     if (t->attr & T_KEY)
          remove(key);
     if (t->attr & T_IV)
          remove(iv);
     if (t->attr & T_VERIFY)
          remove(sign);
#endif

     return RV_OK;
}
