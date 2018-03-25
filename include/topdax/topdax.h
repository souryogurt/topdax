#ifndef TOPDAX_H
#define TOPDAX_H

/** Readable macro to placate compiler */
#define UNUSED(x) (void)(x)

#ifdef __cplusplus
/* *INDENT-OFF* */
extern "C" {
/* *INDENT-ON* */
#endif

/**
 * Runs the application
 * @param @a argc from main() function
 * @param @a argv from main() function
 * @return exit code
 */
int topdax_run(int argc, char **argv);

#ifdef __cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif
#endif
