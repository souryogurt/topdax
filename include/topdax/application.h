#ifndef TOPDAX_APPLICATION_H
#define TOPDAX_APPLICATION_H

#ifdef TOPDAX_USE_PLATFORM_GLFW
#include <GLFW/glfw3.h>
#endif

struct application;

/** Abstract application interface */
struct application_ops {
	/** Specifies callback called when application is started */
	void (*startup) (struct application *);
	/** Specifies callback called when application is activated */
	void (*activate) (struct application *);
	/** Specifies callback called when application is terminated */
	void (*shutdown) (struct application *);
};

/** Abstract application class */
struct application {
	/** Non-zero if application is running, zero otherwise */
	int is_running;
	/** Specifies application implementation */
	const struct application_ops *ops;
};

struct app_window;

/** Abstract application interface */
struct app_window_ops {
	/** Specifies callback called when user closes window */
	void (*close) (struct app_window *);
};

/** Abstract application window class */
struct app_window {
	/** Specifies window implementation */
	const struct app_window_ops *ops;
	/** Specifies window width */
	int width;
	/** Specifies window height */
	int height;
	/** Specifies caption of the window */
	const char *caption;
#ifdef TOPDAX_USE_PLATFORM_GLFW
	/** Instance of GLFW window */
	GLFWwindow *id;
#endif
};

#ifdef __cplusplus
/* *INDENT-OFF* */
extern "C" {
/* *INDENT-ON* */
#endif

/**
 * Runs the application
 * @param app Specifies pointer to application to run
 * @param argc Specifies @a argc from main() function
 * @param argv Specifies @a argv from main() function
 * @return exit code
 */
int application_run(struct application *app, int argc, char **argv);

/**
 * Initializes and shows top-level window
 * @param win Specifies pointer to window to initialize
 * @return zero on success, non-zero otherwise
 */
int app_window_init(struct app_window *win);

/**
 * Requests that the window is closed, similar to what happens when a window
 * manager close button is clicked.
 * @param win Specifies pointer to window to close
 */
void app_window_close(struct app_window *win);

#ifdef __cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif
#endif
