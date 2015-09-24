#ifndef _HOST_DEFS_H
#define _HOST_DEFS_H


#include "config-host.h"
#include <stdint.h>

#if HOST_LONG_BITS == 32
typedef int32_t   host_long;
typedef uint32_t  host_ulong;
#elif HOST_LONG_BITS == 64
typedef int64_t   host_long;
typedef uint64_t  host_ulong;
#endif

#endif 

