
#ifdef __MINGW32__
#include "../config.h"
#define GOOGLE_PERFTOOLS_WINDOWS_CONFIG_H_
#endif

#ifndef GOOGLE_PERFTOOLS_WINDOWS_CONFIG_H_
#define GOOGLE_PERFTOOLS_WINDOWS_CONFIG_H_

#undef WIN32_OVERRIDE_ALLOCATORS

#undef HAVE_SNPRINTF

#undef HAVE_BUILTIN_STACK_POINTER

#undef HAVE_CONFLICT_SIGNAL_H

#undef HAVE_CYGWIN_SIGNAL_H

#undef HAVE_DECL_CFREE

#undef HAVE_DECL_MEMALIGN

#undef HAVE_DECL_POSIX_MEMALIGN

#undef HAVE_DECL_PVALLOC

#undef HAVE_DECL_UNAME

#undef HAVE_DECL_VALLOC

#undef HAVE_DLFCN_H

#undef HAVE_ELF32_VERSYM

#undef HAVE_EXECINFO_H

#undef HAVE_FCNTL_H

#undef HAVE_FEATURES_H

#undef HAVE_GETEUID

#define HAVE_GETPAGESIZE 1   

#undef HAVE_GLOB_H

#undef HAVE_GRP_H

#undef HAVE_INTTYPES_H

#undef HAVE_LIBUNWIND_H

#undef HAVE_LINUX_PTRACE_H

#define HAVE_MALLOC_H 1

#undef HAVE_MALLOC_MALLOC_H

#undef HAVE_MEMORY_H

#undef HAVE_MMAP

#define HAVE_NAMESPACES 1

#undef HAVE_POLL_H

#undef HAVE_PROGRAM_INVOCATION_NAME

#undef HAVE_PTHREAD

#undef HAVE_PWD_H

#undef HAVE_SBRK

#undef HAVE_SCHED_H

#undef HAVE_STDINT_H

#define HAVE_STDLIB_H 1

#undef HAVE_STRINGS_H

#define HAVE_STRING_H 1

#undef HAVE_STRUCT_MALLINFO

#undef HAVE_SYS_CDEFS_H

#undef HAVE_SYS_MALLOC_H

#undef HAVE_SYS_PARAM_H

#undef HAVE_SYS_PRCTL_H

#undef HAVE_SYS_RESOURCE_H

#undef HAVE_SYS_SOCKET_H

#define HAVE_SYS_STAT_H 1

#undef HAVE_SYS_SYSCALL_H

#define HAVE_SYS_TYPES_H 1

#undef HAVE_SYS_UCONTEXT_H

#undef HAVE_SYS_WAIT_H

#define HAVE_TLS 1

#undef HAVE_UCONTEXT_H

#undef HAVE_UNISTD_H

#undef HAVE_UNWIND_H

#undef HAVE_VALGRIND_H

#undef HAVE___ATTRIBUTE__

#undef HAVE___ENVIRON

#define HAVE___INT64 1

#undef INSTALL_PREFIX

#undef INT32_EQUALS_INTPTR

#undef LT_OBJDIR

#undef MALLOC_HOOK_MAYBE_VOLATILE

#undef NO_MINUS_C_MINUS_O

#define PACKAGE "gperftools"

#define PACKAGE_BUGREPORT "opensource@google.com"

#define PACKAGE_NAME "gperftools"

#define PACKAGE_STRING "gperftools 2.0"

#define PACKAGE_TARNAME "gperftools"

#undef PACKAGE_URL

#define PACKAGE_VERSION "2.0"

#undef PC_FROM_UCONTEXT

#ifndef PERFTOOLS_DLL_DECL
# define PERFTOOLS_IS_A_DLL  1   
# define PERFTOOLS_DLL_DECL  __declspec(dllexport)
# define PERFTOOLS_DLL_DECL_FOR_UNITTESTS  __declspec(dllimport)
#endif

#define PRIdS  "Id"

#define PRIuS  "Iu"

#define PRIxS  "Ix"

#ifdef __FreeBSD__
#define PTHREADS_CRASHES_IF_RUN_TOO_EARLY 1
#endif

#undef PTHREAD_CREATE_JOINABLE

#define STDC_HEADERS 1

#define STL_NAMESPACE  std

#undef VERSION

#ifndef __STDC_FORMAT_MACROS
# define __STDC_FORMAT_MACROS 1
#endif

#ifndef __cplusplus
#undef inline
#endif


#ifndef _WIN32_WINNT
# define _WIN32_WINNT 0x0501
#endif

#define NO_HEAP_CHECK 1

#include "windows/port.h"

#endif  
