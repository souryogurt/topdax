#ifndef TOPDAX_THREADS_H
#define TOPDAX_THREADS_H

typedef unsigned long int thrd_t;
typedef int (*thrd_start_t)(void *);

/* Exit and error codes.  */
enum { thrd_success = 0,
       thrd_busy = 1,
       thrd_error = 2,
       thrd_nomem = 3,
       thrd_timedout = 4 };

#ifdef __cplusplus
/* *INDENT-OFF* */
extern "C" {
/* *INDENT-ON* */
#endif

int thrd_create(thrd_t *__thr, thrd_start_t __func, void *__arg);

int thrd_join(thrd_t __thr, int *__res);

#ifdef __cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif
#endif
