#pragma once


typedef enum {
     RV_OK,
     RV_UNKNOWN,
     RV_ALLOCATION_FAILED,
     RV_INVALID_CAVS,
     RV_MECH_NOT_SUPPORTED,
     RV_INVALID_MECHANISM,
     RV_INVALID_ARG,
     RV_EXCEED_THREASHOLD,
     RV_OPEN_FILE_ERROR,
     RV_READ_FILE_ERROR,
     RV_TMP_FILE_ERROR,
     RV_INVALID_KEY_SIZE,
     RV_INVALID_INDEX,
     RV_INVALID_TAG_SIZE,
     RV_INVALID_CONFIG,
     RV_INVALID_RSA
} err_t;

struct err_state {
     const char *file;
     unsigned line;
     err_t errcode;
};

struct err_state g_err_state;

void handle_error(const char *file, unsigned line,  err_t e);

#define error(e) handle_error(__FILE__, __LINE__, e)
#ifdef DEBUG
#define ERR_RET(e)                                      \
     do {                                               \
          g_err_state.file = __FILE__;                  \
          g_err_state.line = __LINE__;                  \
          g_err_state.errcode = e;                      \
          return e;                                     \
     } while (0)

#define ERR_SET(e)                              \
     do {                                       \
          g_err_state.file = __FILE__;          \
          g_err_state.line = __LINE__;          \
          g_err_state.errcode = e;              \
     } while (0)
#else
#define ERR_RET(e)                              \
     return e;

#define ERR_SET(e)
#endif

#ifdef DEBUG
void do_log(const char *fmt, ...);
#else
#define do_log(...);
#endif

#define LOG(fmt, args...) do_log(fmt, ##args)
