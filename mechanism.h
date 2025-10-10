#pragma once
#include "common.h"
#include "parser.h"
#include "tester.h"

err_t aes_cbc_init(global_t *glb, tester_t *t);
bool aes_cbc_verify(global_t *glb, tester_t *t);
err_t aes_ecb_init(global_t *glb, tester_t *t);
bool aes_ecb_verify(global_t *glb, tester_t *t);

bool aes_mct_cbc_swap(tester_t *t);
bool aes_mct_cbc_setkey(tester_t *t, data_t *prekey);
bool aes_mct_cbc_verify(global_t *glb, tester_t *t, data_t *prekey);
bool aes_mct_ecb_swap(tester_t *t);
bool aes_mct_ecb_setkey(tester_t *t, data_t *prekey);
bool aes_mct_ecb_verify(global_t *glb, tester_t *t, data_t *prekey);

err_t tdes_ecb_init(global_t *glb, tester_t *t);
bool tdes_ecb_verify(global_t *glb, tester_t *t);
err_t tdes_cbc_init(global_t *glb, tester_t *t);
bool tdes_cbc_verify(global_t *glb, tester_t *t);

err_t tdes_mmt_ecb_init(global_t *glb, tester_t *t);
bool tdes_mmt_ecb_verify(global_t *glb, tester_t *t);

err_t sha_init(global_t *glb, tester_t *t);
bool sha_verify(global_t *glb, tester_t *t);

err_t rsa_init(global_t *glb, tester_t *t);
bool rsa_verify(global_t *glb, tester_t *t);

err_t rsa_siggen_pkcs_init(global_t *glb, tester_t *t);
bool rsa_siggen_pkcs_verify(global_t *glb, tester_t *t);

err_t hmac_init(global_t *glb, tester_t *t);
bool hmac_verify(global_t *glb, tester_t *t);

typedef err_t (*f_init) (global_t *glb, tester_t *t);
typedef bool (*f_verify) (global_t *glb, tester_t *t);
typedef bool (*f_mct_swap) (tester_t *t);
typedef bool (*f_mct_setkey) (tester_t *t, data_t *prekey);
typedef bool (*f_mct_verify) (global_t *glb, tester_t *t, data_t *prekey);

/**************** (PARAMS) ********************/

enum param_type {
     P_TYPE_NONE,
     P_TYPE_STR,
     P_TYPE_HEX
};

struct alg_param {
     char name[16];
     unsigned init_value;
     enum param_type type;
};

/************* (OPERATOR) **********************/

/*
  opt grammar:
      OPT_O : [ string ]
          e.g. :
                 1. [encrypt]
                 2. [decrypt]

          map to defined const
                 1. encrypt -> OPT_O_ENCRYPT
                 2. decrypt -> OPT_O_DECRYPT

      OPT_V : [ string = string(int) ]
          e.g. :
                 1. [L = 20]
                 2. [mod = 1024]

      OPT_P : string = string(hex)
          e.g. :
                 1. n = e10fa5....
                 2. e = 12345ab....

*/
struct alg_opt {
     unsigned opt;
     char *name;
     struct alg_param *p;
     unsigned param_count;
};

#define OPT(t, n, cls)                         \
     {t, n, cls, sizeof(cls) / sizeof(*cls)}

/*************** (MODES) ***********************/

struct alg_mode {
     char *name;
     char *mech;

     struct alg_opt *opt_list;
     unsigned opt_count;

     f_init test_init;
     f_verify test_verify;
     f_mct_swap test_mct_swap;
     f_mct_setkey test_mct_setkey;
     f_mct_verify test_mct_verify;
};

#define MODE(mode, mech, opt, init, verify, swap, setkey, m_verify)        \
     {mode, mech , opt, sizeof(opt) / sizeof(*opt), init, verify, swap, setkey, m_verify}


/**************** (ALGORITHMS) ****************/


struct algorithm {
     char *name;

     struct alg_mode *modes;
     unsigned mode_count;

     bool is_mct;
};

#define ALG(a, m, is_mct)     \
     {a, m, sizeof(m) / sizeof(*m), is_mct}
