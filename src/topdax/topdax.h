#ifndef TOPDAX_H
#define TOPDAX_H
#include <topdax/application.h>

#ifdef __cplusplus
/* *INDENT-OFF* */
extern "C" {
/* *INDENT-ON* */
#endif

/**
 * Close application window
 * @param obj Specifies pointer to topdax window
 */
void topdax_close_window(struct window_handler *obj, struct window *win);

#ifdef __cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif
#endif
