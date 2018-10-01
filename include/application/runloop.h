#ifndef APPLICATION_RUNLOOP_H
#define APPLICATION_RUNLOOP_H

struct runloop;

/** Runloop operations */
struct runloop_ops {
	/** Specifies pointer to function to quit from runloop */
	void (*quit)(struct runloop *);
};

/** Abstract runloop interface */
struct runloop {
	/** Specifies runloop implementation */
	const struct runloop_ops *ops;
};

#ifdef __cplusplus
/* *INDENT-OFF* */
extern "C" {
/* *INDENT-ON* */
#endif

/** Version string */
extern const char *const g_app_version;

/** Name and email of person responsible for issues */
extern const char *const g_app_bug_address;

/** Application description */
extern const char *const g_app_description;

/**
 * Starts up application components
 * @param loop Specfies loop this application is running on
 * @returns zero on success or non-zero otherwise
 */
int application_startup(struct runloop *loop);

/**
 * Shuts down application components
 */
void application_shutdown(void);

#ifdef __cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif
#endif
