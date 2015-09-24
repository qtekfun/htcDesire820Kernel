


#ifdef EXTENDED_LOGGING
#define MAX_TAG_LEN 128
static char my_portable_tag[MAX_TAG_LEN + 1];

static inline char *portable_tag() {
    extern char *__progname;

    if (my_portable_tag[0] == '\000') {
        strncat(&my_portable_tag[0], __progname, MAX_TAG_LEN);
        strncat(&my_portable_tag[0], ".", MAX_TAG_LEN - strlen(my_portable_tag));
        strncat(&my_portable_tag[0], PORTABLE_TAG, MAX_TAG_LEN - strlen(my_portable_tag));
    }
    return my_portable_tag;
}
#define LOG_TAG  portable_tag()
#else 
#define LOG_TAG PORTABLE_TAG
#endif

#define LOG_PRI(priority, tag, ...) ({                      \
    int _errno = *REAL(__errno)();                          \
    int _rv = android_printLog(priority, tag, __VA_ARGS__); \
    *REAL(__errno)() = _errno;                              \
    _rv;                            \
})

#if !defined(__HOST__)
#include <cutils/log.h>

# define PERROR(str)  {                                                                  \
    ALOGE("%s: PERROR('%s'): errno:%d:'%s'", __func__, str, *REAL(__errno)(), strerror(errno)); \
}

# define ASSERT(cond) ALOG_ASSERT(cond, "assertion failed:(%s), file: %s, line: %d:%s",  \
                                 #cond, __FILE__, __LINE__, __func__);
#else
#include <assert.h>
# define PERROR(str) fprintf(stderr, "%s: PERROR('%s'): errno:%d:'%s'", __func__, str, *REAL(__errno)(), strerror(*REAL(__errno)()))
# define ASSERT(cond) assert(cond)
# define ALOGV(a,...)
# define ALOGW(a,...)
# define ALOGE(a,...)

#endif
