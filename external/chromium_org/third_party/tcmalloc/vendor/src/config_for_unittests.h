// Copyright (c) 2007, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#include "config.h"

#undef PERFTOOLS_DLL_DECL
#ifdef PERFTOOLS_DLL_DECL_FOR_UNITTESTS
# define PERFTOOLS_DLL_DECL  PERFTOOLS_DLL_DECL_FOR_UNITTESTS
#else
# define PERFTOOLS_DLL_DECL  
#endif
