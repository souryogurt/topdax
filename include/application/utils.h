#ifndef APPLICATION_UTILS_H
#define APPLICATION_UTILS_H

/** Readable macro to placate compiler */
#define UNUSED(x) (void)(x)

#ifdef __GNUC__
#define member_type(type, member) __typeof__ (((type *)0)->member)
#else
#define member_type(type, member) const void
#endif

#define container_of(ptr, type, member) ((type *)( \
    (char *)(void*)(member_type(type, member) *){ ptr } - offsetof(type, member)))

/** Returns array size */
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#endif
