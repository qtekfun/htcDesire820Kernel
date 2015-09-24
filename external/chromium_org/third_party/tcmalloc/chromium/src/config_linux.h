


#undef HAVE_CYGWIN_SIGNAL_H

#define HAVE_DECL_CFREE 1

#define HAVE_DECL_MEMALIGN 1

#define HAVE_DECL_POSIX_MEMALIGN 1

#define HAVE_DECL_PVALLOC 1

#define HAVE_DECL_UNAME 1

#define HAVE_DECL_VALLOC 1

#define HAVE_DLFCN_H 1

#define HAVE_ELF32_VERSYM 1

#define HAVE_EXECINFO_H 1

#define HAVE_FCNTL_H 1

#define HAVE_FEATURES_H 1

#define HAVE_GETEUID 1

#define HAVE_GETPAGESIZE 1

#define HAVE_GLOB_H 1

#define HAVE_GRP_H 1

#define HAVE_INTTYPES_H 1


#define HAVE_LINUX_PTRACE_H 1

#define HAVE_MALLOC_H 1

#undef HAVE_MALLOC_MALLOC_H

#define HAVE_MEMORY_H 1

#define HAVE_MMAP 1

#define HAVE_NAMESPACES 1

#define HAVE_POLL_H 1

#define HAVE_PROGRAM_INVOCATION_NAME 1

#define HAVE_PTHREAD 1

#define HAVE_PWD_H 1

#define HAVE_SBRK 1

#define HAVE_SCHED_H 1

#define HAVE_STDINT_H 1

#define HAVE_STDLIB_H 1

#define HAVE_STRINGS_H 1

#define HAVE_STRING_H 1

#define HAVE_STRUCT_MALLINFO 1

#define HAVE_SYS_CDEFS_H 1

#undef HAVE_SYS_MALLOC_H

#define HAVE_SYS_PARAM_H 1

#define HAVE_SYS_PRCTL_H 1

#define HAVE_SYS_RESOURCE_H 1

#define HAVE_SYS_SOCKET_H 1

#define HAVE_SYS_STAT_H 1

#define HAVE_SYS_SYSCALL_H 1

#define HAVE_SYS_TIME_H 1

#define HAVE_SYS_TYPES_H 1

#undef HAVE_SYS_UCONTEXT_H

#define HAVE_SYS_WAIT_H 1

#define HAVE_TLS 1

#define HAVE_UCONTEXT_H 1

#define HAVE_UNISTD_H 1

#define HAVE_UNWIND_H 1

#undef HAVE_VALGRIND_H

#define HAVE___ATTRIBUTE__ 1

#undef HAVE___ENVIRON


#define INSTALL_PREFIX "/usr/local"


#undef LT_OBJDIR


#define PACKAGE "google-perftools"

#define PACKAGE_BUGREPORT "opensource@google.com"

#define PACKAGE_NAME "google-perftools"

#define PACKAGE_STRING "google-perftools 1.7"

#define PACKAGE_TARNAME "google-perftools"

#undef PACKAGE_URL

#define PACKAGE_VERSION "1.7"

#if defined(__arm__)
#define PC_FROM_UCONTEXT uc_mcontext.arm_pc
#else
#define PC_FROM_UCONTEXT uc_mcontext.gregs[REG_RIP]
#endif

#define PERFTOOLS_DLL_DECL 

#define PRIdS "zd"

#define PRIuS "zu"

#define PRIxS "zx"


#define STDC_HEADERS 1

#define STL_NAMESPACE std

#define VERSION "1.7"

#ifndef __STDC_FORMAT_MACROS
# define __STDC_FORMAT_MACROS 1
#endif

#ifndef __cplusplus
#endif


#ifdef __MINGW32__
#include "windows/mingw.h"
#endif
