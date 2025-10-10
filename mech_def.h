#pragma once

#include "mechanism.h"

struct alg_param param_class_1[] = {
     {"count", 0, P_TYPE_NONE},
     {"key", 1024, P_TYPE_HEX},
     {"iv", 1024, P_TYPE_HEX},
     {"plaintext", 1024, P_TYPE_HEX},
     {"ciphertext", 1024, P_TYPE_HEX},
};

struct alg_param param_class_2[] = {
     {"count", 0, P_TYPE_NONE},
     {"key", 1024, P_TYPE_HEX},
     {"iv", 1024, P_TYPE_HEX},
     {"ciphertext", 1024, P_TYPE_HEX},
     {"plaintext", 1024, P_TYPE_HEX}
};

struct alg_param param_class_3[] = {
     {"count", 0, P_TYPE_NONE},
     {"key", 1024, P_TYPE_HEX},
     {"plaintext", 1024, P_TYPE_HEX},
     {"ciphertext", 1024, P_TYPE_HEX}
};

struct alg_param param_class_4[] = {
     {"count", 0, P_TYPE_HEX},
     {"key", 1024, P_TYPE_HEX},
     {"ciphertext", 1024, P_TYPE_HEX},
     {"plaintext", 1024, P_TYPE_HEX}
};

struct alg_param param_class_5[] = {
     {"count", 0, P_TYPE_NONE},
     {"keys", 1024, P_TYPE_HEX},
     {"plaintext", 1024, P_TYPE_HEX},
     {"ciphertext", 1024, P_TYPE_HEX}
};

struct alg_param param_class_6[] = {
     {"count", 0, P_TYPE_NONE},
     {"keys", 1024, P_TYPE_HEX},
     {"ciphertext", 1024, P_TYPE_HEX},
     {"plaintext",  1024, P_TYPE_HEX}
};

struct alg_param param_class_7[] = {
     {"count", 0, P_TYPE_NONE},
     {"keys", 1024, P_TYPE_HEX},
     {"iv", 1024, P_TYPE_HEX},
     {"plaintext", 1024, P_TYPE_HEX},
     {"ciphertext", 1024, P_TYPE_HEX}
};

struct alg_param param_class_8[] = {
     {"count", 0, P_TYPE_NONE},
     {"keys", 1024, P_TYPE_HEX},
     {"iv", 1024, P_TYPE_HEX},
     {"ciphertext", 1024, P_TYPE_HEX},
     {"plaintext", 1024, P_TYPE_HEX}
};

struct alg_param param_class_9[] = {
     {"len", 0, P_TYPE_NONE},
     {"msg", 4096, P_TYPE_HEX},
     {"md", 32, P_TYPE_HEX}
};

struct alg_param param_class_10[] = {
     {"count", 0, P_TYPE_NONE},
     {"n", 256, P_TYPE_HEX},
     {"p", 256, P_TYPE_HEX},
     {"q", 256, P_TYPE_HEX},
     {"e", 256, P_TYPE_HEX},
     {"d", 256, P_TYPE_HEX},
     {"em", 256, P_TYPE_HEX},
     {"s", 256, P_TYPE_HEX}
};

struct alg_param param_class_11[] = {
     {"count", 0, P_TYPE_NONE},
     {"klen", 0, P_TYPE_HEX},
     {"tlen", 0, P_TYPE_HEX},
     {"key", 1024, P_TYPE_HEX},
     {"msg", 256, P_TYPE_HEX},
     {"mac", 256, P_TYPE_HEX}
};

struct alg_param param_class_12[] = {
     {"count", 0, P_TYPE_NONE},
     {"key1", 1024, P_TYPE_HEX},
     {"key2", 1024, P_TYPE_HEX},
     {"key3", 1024, P_TYPE_HEX},
     {"plaintext", 1024, P_TYPE_HEX},
     {"ciphertext", 1024, P_TYPE_HEX}
};

struct alg_param param_class_13[] = {
     {"count", 0, P_TYPE_NONE},
     {"key1", 1024, P_TYPE_HEX},
     {"key2", 1024, P_TYPE_HEX},
     {"key3", 1024, P_TYPE_HEX},
     {"ciphertext", 1024, P_TYPE_HEX},
     {"plaintext", 1024, P_TYPE_HEX},
};

struct alg_param param_class_14[] = {
     {"SHAAlg", 16, P_TYPE_STR},
     {"msg", 512, P_TYPE_HEX},
     {"s", 1024, P_TYPE_HEX}
};




struct alg_opt sha_opt[] = {
     OPT(OPT_NONE, "", param_class_9)
};

struct alg_opt aes_cbc_opt[] = {
     OPT(OPT_O_ENCRYPT, "encrypt", param_class_1),
     OPT(OPT_O_DECRYPT, "decrypt", param_class_2)
};

struct alg_opt aes_ecb_opt[] = {
     OPT(OPT_O_ENCRYPT, "encrypt", param_class_3),
     OPT(OPT_O_DECRYPT, "decrypt", param_class_4)
};

struct alg_opt tdes_ecb_opt[] = {
     OPT(OPT_O_ENCRYPT, "encrypt", param_class_5),
     OPT(OPT_O_DECRYPT, "decrypt", param_class_6)
};

struct alg_opt tdes_cbc_opt[] = {
     OPT(OPT_O_ENCRYPT, "encrypt", param_class_7),
     OPT(OPT_O_DECRYPT, "decrypt", param_class_8)
};

struct alg_opt tdes_mmt_ecb_opt[] = {
     OPT(OPT_O_ENCRYPT, "encrypt", param_class_12),
     OPT(OPT_O_DECRYPT, "decrypt", param_class_13)
};

struct alg_opt rsa_opt[] = {
     OPT(OPT_NONE, "", param_class_10)
};

struct alg_opt hmac_opt[] = {
     OPT(OPT_V, "L", param_class_11)
};

struct alg_opt rsa_siggen_pkcs_opt[] = {
     OPT(OPT_V, "mod", param_class_14),
     OPT(OPT_P, "n", NULL),
     OPT(OPT_P, "e", NULL),
     OPT(OPT_P, "d", NULL)
};




struct alg_mode sha_modes[] = {
     MODE("SHA-1 LongMsg", "CKM_SHA_1", sha_opt, sha_init, sha_verify,
          NULL, NULL, NULL),
     MODE("SHA-1 ShortMsg", "CKM_SHA_1", sha_opt, sha_init, sha_verify,
          NULL, NULL, NULL),
     MODE("SHA-224 LongMsg", "CKM_SHA224", sha_opt, sha_init, sha_verify,
          NULL, NULL, NULL),
     MODE("SHA-224 ShortMsg", "CKM_SHA224", sha_opt, sha_init, sha_verify,
          NULL, NULL, NULL),
     MODE("SHA-256 LongMsg", "CKM_SHA256", sha_opt, sha_init, sha_verify,
          NULL, NULL, NULL),
     MODE("SHA-256 ShortMsg", "CKM_SHA256", sha_opt, sha_init, sha_verify,
          NULL, NULL, NULL),
     MODE("SHA-384 LongMsg", "CKM_SHA384", sha_opt, sha_init, sha_verify,
          NULL, NULL, NULL),
     MODE("SHA-384 ShortMsg", "CKM_SHA384", sha_opt, sha_init, sha_verify,
          NULL, NULL, NULL),
     MODE("SHA-512 LongMsg", "CKM_SHA512", sha_opt, sha_init, sha_verify,
          NULL, NULL, NULL),
     MODE("SHA-512 ShortMsg", "CKM_SHA512", sha_opt, sha_init, sha_verify,
          NULL, NULL, NULL)
};

struct alg_mode aes_modes[]  = {
     MODE("CBC", "CKM_AES_CBC", aes_cbc_opt, aes_cbc_init, aes_cbc_verify,
          NULL, NULL, NULL),
     MODE("ECB", "CKM_AES_ECB", aes_ecb_opt, aes_ecb_init, aes_ecb_verify,
          NULL, NULL, NULL),
     MODE("CFB128", "CKM_AES_CFB128", aes_cbc_opt, aes_cbc_init, aes_cbc_verify,
          NULL, NULL, NULL),
     MODE("OFB", "CKM_AES_OFB", aes_cbc_opt, aes_cbc_init, aes_cbc_verify,
          NULL, NULL, NULL)
};

struct alg_mode aes_mct_modes[] = {
     /* MODE("CBC", "CKM_AES_CBC", aes_cbc_opt, aes_cbc_init, NULL, */
     /*      aes_mct_cbc_swap, aes_mct_cbc_setkey, aes_mct_cbc_verify), */
     MODE("ECB", "CKM_AES_ECB", aes_ecb_opt, aes_ecb_init, NULL,
          aes_mct_ecb_swap, aes_mct_ecb_setkey, aes_mct_ecb_verify)
};

struct alg_mode tdes_modes[] = {
     MODE("ECB", "CKM_DES3_ECB", tdes_ecb_opt, tdes_ecb_init, tdes_ecb_verify,
          NULL, NULL, NULL),
     MODE("CBC", "CKM_DES3_CBC", tdes_cbc_opt, tdes_cbc_init, tdes_cbc_verify,
          NULL, NULL, NULL),
     MODE("CFB64", "CKM_DES3_CFB64", tdes_cbc_opt, tdes_cbc_init, tdes_cbc_verify,
          NULL, NULL, NULL),
     MODE("OFB", "CKM_DES3_OFB64", tdes_cbc_opt, tdes_cbc_init, tdes_cbc_verify,
          NULL, NULL, NULL)
};

struct alg_mode tdes_mmt_modes[] = {
     MODE("Multi block Message Test for ECB", "CKM_DES3_ECB", tdes_mmt_ecb_opt,
          tdes_mmt_ecb_init, tdes_mmt_ecb_verify, NULL, NULL, NULL)
};

struct alg_mode rsa_modes[] = {
     MODE("RSASP1", "CKM_RSA_X_509", rsa_opt, rsa_init, rsa_verify,
          NULL, NULL, NULL)
};

struct alg_mode hmac_modes[] = {
     MODE("HMAC", "", hmac_opt, hmac_init, hmac_verify, NULL, NULL, NULL)
};

struct alg_mode rsa_siggen_modes[] = {
     MODE("PKCS", "", rsa_siggen_pkcs_opt, rsa_siggen_pkcs_init,
          rsa_siggen_pkcs_verify, NULL, NULL, NULL)
};

struct algorithm alg_list[] = {
     ALG("SHA", sha_modes, false),
     ALG("AESVS MCT", aes_mct_modes, true),
     ALG("AESVS", aes_modes, false),
     ALG("TDES", tdes_mmt_modes, false),
     ALG("tdes", tdes_modes, false),
     ALG("RSA", rsa_modes, false),
     ALG("HMAC", hmac_modes, false),
     ALG("SigGen", rsa_siggen_modes, false)
};
