
#define HASH_MAP_CLASS hash_map

#if defined(USE_STLPORT)
#define HASH_MAP_H <hash_map>
#else
#define HASH_MAP_H <ext/hash_map>
#endif

#if defined(USE_STLPORT)
#define HASH_NAMESPACE std
#else
#define HASH_NAMESPACE __gnu_cxx
#endif

#define HASH_SET_CLASS hash_set

#if defined(USE_STLPORT)
#define HASH_SET_H <hash_set>
#else
#define HASH_SET_H <ext/hash_set>
#endif

#define HAVE_DLFCN_H 1

#define HAVE_FCNTL_H 1

#define HAVE_FTRUNCATE 1

#define HAVE_HASH_MAP 1

#define HAVE_HASH_SET 1

#define HAVE_INTTYPES_H 1

#define HAVE_LIMITS_H 1

#define HAVE_MEMORY_H 1

#define HAVE_MEMSET 1

#define HAVE_MKDIR 1

#define HAVE_PTHREAD 1

#define HAVE_STDINT_H 1

#define HAVE_STDLIB_H 1

#define HAVE_STRCHR 1

#define HAVE_STRERROR 1

#define HAVE_STRINGS_H 1

#define HAVE_STRING_H 1

#define HAVE_STRTOL 1

#define HAVE_SYS_STAT_H 1

#define HAVE_SYS_TYPES_H 1

#define HAVE_UNISTD_H 1

#define HAVE_ZLIB 1

#define PACKAGE "protobuf"

#define PACKAGE_BUGREPORT "protobuf@googlegroups.com"

#define PACKAGE_NAME "Protocol Buffers"

#define PACKAGE_STRING "Protocol Buffers 2.3.0"

#define PACKAGE_TARNAME "protobuf"

#define PACKAGE_VERSION "2.3.0"


#define STDC_HEADERS 1

#define VERSION "2.3.0"

#ifndef _ALL_SOURCE
#endif

#ifndef _GNU_SOURCE
# define _GNU_SOURCE 1
#endif




#ifndef __EXTENSIONS__
# define __EXTENSIONS__ 1
#endif
#ifndef _POSIX_PTHREAD_SEMANTICS
# define _POSIX_PTHREAD_SEMANTICS 1
#endif
#ifndef _TANDEM_SOURCE
# define _TANDEM_SOURCE 1
#endif
