#ifdef CONFIG_TRACE
DEF_HELPER_1(traceTicks, void, i32)
DEF_HELPER_0(traceInsn, void)
#if HOST_LONG_BITS == 32
DEF_HELPER_2(traceBB32, void, i64, i32)
#endif
#if HOST_LONG_BITS == 64
DEF_HELPER_2(traceBB64, void, i64, i64)
#endif
#endif

#ifdef CONFIG_MEMCHECK
DEF_HELPER_2(on_call, void, i32, i32)
DEF_HELPER_1(on_ret, void, i32)
#endif  
#include "def-helper.h"
