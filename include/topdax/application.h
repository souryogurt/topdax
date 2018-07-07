#ifndef TOPDAX_APPLICATION_H
#define TOPDAX_APPLICATION_H

/** Readable macro to placate compiler */
#define UNUSED(x) (void)(x)

#ifdef __GNUC__
#define member_type(type, member) __typeof__ (((type *)0)->member)
#else
#define member_type(type, member) const void
#endif

#define container_of(ptr, type, member) ((type *)( \
    (char *)(void*)(member_type(type, member) *){ ptr } - offsetof(type, member)))

struct runloop;

/** Runloop operations */
struct runloop_ops {
	/** Specifies pointer to function to quit from runloop */
	void (*quit) (struct runloop *);
};

/** Abstract runloop interface */
struct runloop {
	/** Specifies runloop implementation */
	const struct runloop_ops *ops;
};

/** Application operations */
struct application_ops {
	/** Specifies callback called when application is started */
	void (*startup) (struct runloop *);
	/** Specifies callback called when application is activated */
	void (*activate) (void);
	/** Specifies callback called when application is terminated */
	void (*shutdown) (void);
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

#ifdef __cplusplus
/* *INDENT-OFF* */
extern "C" {
/* *INDENT-ON* */
#endif

#ifdef __cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif
#endif
