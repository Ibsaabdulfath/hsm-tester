#pragma once

#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include "data.h"

/*
#define KEY_INIT    512
#define PLAIN_INIT  4096
#define CIPHER_INIT 4096
*/

#define OPT_NONE 0

#define OPT_O (1 << 4)
#define OPT_V (1 << 5)
#define OPT_P (1 << 6)

#define OPT_O_ENCRYPT (OPT_O | 1)  /* b#00010001 */
#define OPT_O_DECRYPT (OPT_O | 2)  /* b#00010010 */

typedef struct {
     data_t *g_params;
     uint16_t g_count;
     uint16_t flag;
     unsigned value;
} opt_t;

typedef struct {
     char *tester;
     char *pkcs11;
     char *pin;
} config_t;

typedef struct {
     data_t *params;
     unsigned param_count;

     unsigned alg_index;

     struct alg_mode *mode;
     char *mech;

     opt_t opt;

     unsigned keysize;

     pid_t p_pid;

     config_t config;

     bool is_mct;
} global_t;
