#ifndef TOPDAX_APPLICATION_H
#define TOPDAX_APPLICATION_H

#ifdef TOPDAX_USE_PLATFORM_GLFW
#include <GLFW/glfw3.h>
#endif

/** Readable macro to placate compiler */
#define UNUSED(x) (void)(x)

#ifdef __GNUC__
#define member_type(type, member) __typeof__ (((type *)0)->member)
#else
#define member_type(type, member) const void
#endif

#define container_of(ptr, type, member) ((type *)( \
    (char *)(void*)(member_type(type, member) *){ ptr } - offsetof(type, member)))

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
	/** zero if application is running, non-zero otherwise */
	int must_quit;
};

struct window;

/** Window operations */
struct window_ops {
	/** Specifies pointer to function to request window close */
	void (*close) (struct window *);
};

/** Abstract window interface */
struct window {
	/** Specifies window implementation */
	const struct window_ops *ops;
};

struct window_handler;

/** Window handler operations */
struct window_handler_ops {
	/** Specifies pointer to function that called when window is closed */
	void (*close) (struct window_handler *, struct window *);
};

/** Abstract window handler interface */
struct window_handler {
	/** Specifies window handler implementation */
	const struct window_handler_ops *ops;
};

/** Window implementation using GLFW API */
struct glfw_window {
	/** Implements window interface */
	struct window win;
	/** Instance of GLFW window */
	GLFWwindow *id;
	/** Pointer to window handler implementation */
	struct window_handler *handler;
};

#ifdef __cplusplus
/* *INDENT-OFF* */
extern "C" {
/* *INDENT-ON* */
#endif

/**
 * Runs the application
 * @param app Specifies pointer to application to run
 * @param info Specifies pointer to application info
 * @param argc Specifies @a argc from main() function
 * @param argv Specifies @a argv from main() function
 * @return exit code
 */
int application_run(struct application *app,
		    const struct application_info *info, int argc, char **argv);

/**
 * Quits the application
 * @param app Specifies pointer to application to quit
 */
void application_quit(struct application *app);

/**
 * Initializes and shows top-level window using GLFW
 * @param win Specifies pointer to glfw_window to initialize
 * @param width Specifies initial width of window
 * @param height Specifies initial height of window
 * @param caption Specifies initial caption of window
 * @param wh Specifies pointer to window handler that will respond to events
 * @return zero on success, non-zero otherwise
 */
int glfw_window_init(struct glfw_window *win, int width, int height,
		     const char *caption, struct window_handler *wh);

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
