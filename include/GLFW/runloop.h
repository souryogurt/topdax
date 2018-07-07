#ifndef GLFW_RUNLOOP_H
#define GLFW_RUNLOOP_H

#include <topdax/application.h>

/** Runloop implementation using GLFW API */
struct glfw_runloop {
	/** Implements runloop interface */
	struct runloop loop;
	/** zero if application is running, non-zero otherwise */
	int must_quit;
};

#ifdef __cplusplus
/* *INDENT-OFF* */
extern "C" {
/* *INDENT-ON* */
#endif

/**
 * Runs the application
 * @param loop Specifies pointer to runloop to start
 * @param argc Specifies @a argc from main() function
 * @param argv Specifies @a argv from main() function
 * @return exit code
 */
int glfw_runloop_run(struct glfw_runloop *loop, int argc, char **argv);

#ifdef __cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif
#endif
