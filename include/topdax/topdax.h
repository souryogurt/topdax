#ifndef TOPDAX_H
#define TOPDAX_H
#include "topdax/application.h"

#ifdef __cplusplus
/* *INDENT-OFF* */
extern "C" {
/* *INDENT-ON* */
#endif

/** Implementation of Topdax application operations */
const struct application_ops topdax_ops;

/**
 * Close application window
 * @param obj Specifies pointer to topdax window
 */
void topdax_close_window(struct window_handler *obj, struct window *win);

/**
 * Runs the application
 * @param argc @a argc from main() function
 * @param argv @a argv from main() function
 * @return exit code
 */
int topdax_run(int argc, char **argv);

#ifdef __cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif
#endif
