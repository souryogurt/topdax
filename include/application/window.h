#ifndef APPLICATION_WINDOW_H
#define APPLICATION_WINDOW_H

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

#endif
