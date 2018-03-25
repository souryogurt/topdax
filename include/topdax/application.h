#ifndef TOPDAX_APPLICATION_H
#define TOPDAX_APPLICATION_H

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

#ifdef __cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif
#endif
