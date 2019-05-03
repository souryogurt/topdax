/**
 * @file
 * C11 threads emulation using pthread
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "c11threads.h"
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

struct impl_thrd_param {
	thrd_start_t func;
	void *arg;
};

static void *impl_thrd_routine(void *p)
{
	struct impl_thrd_param pack = *((struct impl_thrd_param *)p);
	free(p);
	return (void *)(intptr_t)pack.func(pack.arg);
}

int thrd_create(thrd_t *__thr, thrd_start_t __func, void *__arg)
{
	struct impl_thrd_param *pack;
	pack = (struct impl_thrd_param *)malloc(sizeof(struct impl_thrd_param));
	if (!pack)
		return thrd_nomem;
	pack->func = __func;
	pack->arg = __arg;
	if (pthread_create(__thr, NULL, impl_thrd_routine, pack) != 0) {
		free(pack);
		return thrd_error;
	}
	return thrd_success;
}

int thrd_join(thrd_t __thr, int *__res)
{
	void *code;
	if (pthread_join(__thr, &code) != 0)
		return thrd_error;
	if (__res)
		*__res = (int)(intptr_t)code;
	return thrd_success;
}
