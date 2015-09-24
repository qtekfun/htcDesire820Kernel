#include "android/config/config.h"
#define TARGET_I386 1
#ifdef CONFIG_LINUX
#define CONFIG_KVM  1
#endif

#ifdef CONFIG_DARWIN
#define CONFIG_HAX 1
#endif

#ifdef CONFIG_WIN32
#define CONFIG_HAX 1
#endif
