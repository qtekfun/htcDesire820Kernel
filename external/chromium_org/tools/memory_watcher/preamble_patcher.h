// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef MEMORY_WATCHER_PREAMBLE_PATCHER_H__
#define MEMORY_WATCHER_PREAMBLE_PATCHER_H__

#include <windows.h>

#include "base/logging.h"
#define ASSERT(cond, msg)  DCHECK(cond)
#define ASSERT1(cond)      DCHECK(cond)

#define MAX_PREAMBLE_STUB_SIZE    (32)

namespace sidestep {

enum SideStepError {
  SIDESTEP_SUCCESS = 0,
  SIDESTEP_INVALID_PARAMETER,
  SIDESTEP_INSUFFICIENT_BUFFER,
  SIDESTEP_JUMP_INSTRUCTION,
  SIDESTEP_FUNCTION_TOO_SMALL,
  SIDESTEP_UNSUPPORTED_INSTRUCTION,
  SIDESTEP_NO_SUCH_MODULE,
  SIDESTEP_NO_SUCH_FUNCTION,
  SIDESTEP_ACCESS_DENIED,
  SIDESTEP_UNEXPECTED,
};

#define SIDESTEP_TO_HRESULT(error) \
  MAKE_HRESULT(SEVERITY_ERROR, FACILITY_NULL, error)

class PreamblePatcher {
 public:

  
  
  
  
  
  
  
  
  
  
  
  
  template <class T>
  static SideStepError Patch(T target_function,
                               T replacement_function,
                               T* original_function_stub) {
    
    
    
    return RawPatch((void*)(target_function),
                    (void*)(replacement_function),
                    (void**)(original_function_stub));
  }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  template <class T>
  static SideStepError Patch(LPCTSTR module_name,
                               LPCSTR function_name,
                               T replacement_function,
                               T* original_function_stub) {
    ASSERT1(module_name && function_name);
    if (!module_name || !function_name) {
      ASSERT(false,
             "You must specify a module name and function name.");
      return SIDESTEP_INVALID_PARAMETER;
    }
    HMODULE module = ::GetModuleHandle(module_name);
    ASSERT1(module != NULL);
    if (!module) {
      ASSERT(false, "Invalid module name.");
      return SIDESTEP_NO_SUCH_MODULE;
    }
    FARPROC existing_function = ::GetProcAddress(module, function_name);
    if (!existing_function) {
      return SIDESTEP_NO_SUCH_FUNCTION;
    }
    
    
    
    return RawPatch((void*)existing_function, (void*)replacement_function,
                    (void**)(original_function_stub));
  }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static SideStepError RawPatch(void* target_function,
                                  void* replacement_function,
                                  void** original_function_stub);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static SideStepError Unpatch(void* target_function,
                               void* replacement_function,
                               void* original_function_stub);

 private:

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static SideStepError RawPatchWithStubAndProtections(void* target_function,
                                          void *replacement_function,
                                          unsigned char* preamble_stub,
                                          unsigned long stub_size,
                                          unsigned long* bytes_needed);

  
  
  
  static SideStepError RawPatchWithStub(void* target_function,
                                          void *replacement_function,
                                          unsigned char* preamble_stub,
                                          unsigned long stub_size,
                                          unsigned long* bytes_needed);
};

};  

#endif  
