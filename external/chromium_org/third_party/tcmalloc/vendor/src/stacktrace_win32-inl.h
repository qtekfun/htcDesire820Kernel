// Copyright (c) 2008, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT

#ifndef BASE_STACKTRACE_WIN32_INL_H_
#define BASE_STACKTRACE_WIN32_INL_H_

#include "config.h"
#include <windows.h>    
#include <assert.h>

typedef USHORT NTAPI RtlCaptureStackBackTrace_Function(
    IN ULONG frames_to_skip,
    IN ULONG frames_to_capture,
    OUT PVOID *backtrace,
    OUT PULONG backtrace_hash);

static RtlCaptureStackBackTrace_Function* const RtlCaptureStackBackTrace_fn =
   (RtlCaptureStackBackTrace_Function*)
   GetProcAddress(GetModuleHandleA("ntdll.dll"), "RtlCaptureStackBackTrace");

PERFTOOLS_DLL_DECL int GetStackTrace(void** result, int max_depth,
                                     int skip_count) {
  if (!RtlCaptureStackBackTrace_fn) {
    
    return 0;     
  }
  return (int)RtlCaptureStackBackTrace_fn(skip_count + 2, max_depth,
                                          result, 0);
}

PERFTOOLS_DLL_DECL int GetStackFrames(void** ,
                                      int* ,
                                      int ,
                                      int ) {
  assert(0 == "Not yet implemented");
  return 0;
}

#endif  
