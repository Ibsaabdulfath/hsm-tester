#pragma once
#include "common.h"
#include "err.h"

#define T_IV      1
#define T_KEY     2
#define T_HASH    4
#define T_ENCRYPT 8
#define T_DECRYPT 16
#define T_SIGN    32
#define T_VERIFY  64

typedef struct {
     data_t *iv;
     data_t *plain;
     data_t *key;
     data_t *sign;
     unsigned keysize;

     data_t *out;

     unsigned attr;
} tester_t;

err_t tester_run(global_t *glb, tester_t *t);
