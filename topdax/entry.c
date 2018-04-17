/**
 * @file
 * Application entrypoint
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "topdax/topdax.h"

/** Entry point */
int main(int argc, char **argv)
{
	return topdax_run(argc, argv);
}