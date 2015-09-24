// Copyright (c) 2005, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GOOGLE_STACKTRACE_H_
#define GOOGLE_STACKTRACE_H_

#ifndef PERFTOOLS_DLL_DECL
# ifdef _WIN32
#   define PERFTOOLS_DLL_DECL  __declspec(dllimport)
# else
#   define PERFTOOLS_DLL_DECL
# endif
#endif


extern PERFTOOLS_DLL_DECL int GetStackFrames(void** result, int* sizes, int max_depth,
                          int skip_count);

extern PERFTOOLS_DLL_DECL int GetStackFramesWithContext(void** result, int* sizes, int max_depth,
                                     int skip_count, const void *uc);

extern PERFTOOLS_DLL_DECL int GetStackTrace(void** result, int max_depth,
                                            int skip_count);

extern PERFTOOLS_DLL_DECL int GetStackTraceWithContext(void** result, int max_depth,
                                    int skip_count, const void *uc);

#endif 
