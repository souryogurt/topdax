/**
 * @file
 * Application entrypoint
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <GLFW/runloop.h>

/** Entry point */
int main(int argc, char **argv)
{
	return glfw_runloop_run(argc, argv);
}
