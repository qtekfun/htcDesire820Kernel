
#ifdef __MINGW32__
#include "../config.h"
#define GOOGLE_PERFTOOLS_WINDOWS_CONFIG_H_
#endif

#ifndef GOOGLE_PERFTOOLS_WINDOWS_CONFIG_H_
#define GOOGLE_PERFTOOLS_WINDOWS_CONFIG_H_

#define WIN32_OVERRIDE_ALLOCATORS

#define HASH_MAP_H  <hash_map>

#define HASH_NAMESPACE  stdext

#define HASH_SET_H  <hash_set>

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

#define HAVE_FCNTL_H 1

#undef HAVE_FEATURES_H

#undef HAVE_GETEUID

#define HAVE_GETPAGESIZE 1   

#undef HAVE_GLOB_H

#undef HAVE_GRP_H

#define HAVE_HASH_MAP 1

#define HAVE_HASH_SET 1

#undef HAVE_INTTYPES_H

#undef HAVE_LIBUNWIND_H

#undef HAVE_LINUX_PTRACE_H

#undef HAVE_MALLOC_H

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

#undef HAVE_SYS_TIME_H

#define HAVE_SYS_TYPES_H 1

#undef HAVE_SYS_UCONTEXT_H

#undef HAVE_SYS_WAIT_H

#undef HAVE_TLS

#undef HAVE_UCONTEXT_H

#undef HAVE_UNISTD_H

#undef HAVE_UNWIND_H

#undef HAVE___ATTRIBUTE__

#undef HAVE___ENVIRON

#define HAVE___INT64 1

#undef INSTALL_PREFIX

#undef INT32_EQUALS_INTPTR

#undef LT_OBJDIR

#undef NO_MINUS_C_MINUS_O

#undef PACKAGE

#undef PACKAGE_BUGREPORT

#undef PACKAGE_NAME

#undef PACKAGE_STRING

#undef PACKAGE_TARNAME

#undef PACKAGE_URL

#undef PACKAGE_VERSION

#undef PC_FROM_UCONTEXT

#ifndef PERFTOOLS_DLL_DECL
# define PERFTOOLS_IS_A_DLL  1   
# define PERFTOOLS_DLL_DECL  __declspec(dllexport)
# define PERFTOOLS_DLL_DECL_FOR_UNITTESTS  __declspec(dllimport)
#endif

#define PRIdS  "Id"

#define PRIuS  "Iu"

#define PRIxS  "Ix"

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
