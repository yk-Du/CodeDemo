#ifndef __dbg_h
#define __dbg_h

#include <stdio.h>
#include <errno.h>
#include <string.h>

#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) fprintf(stderr, "DEBUG %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

// file:line can be directly open by vim copen buff
#define log_e(M, ...) fprintf(stderr,  "%s:%d:%s:\t[ERR]  errono: %s | " M "\n", __FILE__, __LINE__, __FUNCTION__, clean_errno(), ##__VA_ARGS__)
#define log_w(M, ...) fprintf(stderr, "%s:%d:%s:\t[WAR]  errono: %s | " M "\n", __FILE__, __LINE__, __FUNCTION__, clean_errno(), ##__VA_ARGS__)
#define log_i(M, ...) fprintf(stderr, "%s:%d:%s:\t[INF]  errono: %s | " M "\n", __FILE__, __LINE__, __FUNCTION__, clean_errno(), ##__VA_ARGS__)

#define check(A, M, ...) if(!(A)) { log_e(M, ##__VA_ARGS__); errno = 0; goto error; }

#define sentinel(M, ...) { log_e(M, ##__VA_ARGS__); errno = 0; goto error; }

#define check_mem(A) { check(A, "Out of Memory.") }

#define check_debug(A, M, ...) if(!(A)) { debug(M, ##__VA_ARGS__); errno = 0; goto error; }

#endif
