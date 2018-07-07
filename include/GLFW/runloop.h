#ifndef GLFW_RUNLOOP_H
#define GLFW_RUNLOOP_H

#ifdef __cplusplus
/* *INDENT-OFF* */
extern "C" {
/* *INDENT-ON* */
#endif

/**
 * Runs the application
 * @param argc Specifies @a argc from main() function
 * @param argv Specifies @a argv from main() function
 * @return exit code
 */
int glfw_runloop_run(int argc, char **argv);

#ifdef __cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif
#endif
