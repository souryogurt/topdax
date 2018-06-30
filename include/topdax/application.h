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

/** Application information */
struct application_info {
	/** Version string */
	const char *version;
	/** Name and email of person responsible for issues */
	const char *bug_address;
	/** Application summary */
	const char *summary;
};

/** Abstract application class */
struct application {
	/** Specifies application implementation */
	const struct application_ops *ops;
	/** Information about application */
	const struct application_info *info;
	/** zero if application is running, non-zero otherwise */
	int must_quit;
};

struct window;

/** Abstract application interface */
struct window_ops {
	/** Specifies callback called when user closes window */
	void (*close) (struct window *);
};

/** Abstract application window class */
struct window {
	/** Specifies window implementation */
	const struct window_ops *ops;
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
 * Quits the application
 * @param app Specifies pointer to application to quit
 */
void application_quit(struct application *app);

/**
 * Initializes and shows top-level window
 * @param win Specifies pointer to window to initialize
 * @return zero on success, non-zero otherwise
 */
int window_init(struct window *win);

/**
 * Requests that the window is closed, similar to what happens when a window
 * manager close button is clicked.
 * @param win Specifies pointer to window to close
 */
void window_close(struct window *win);

#ifdef __cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif
#endif
