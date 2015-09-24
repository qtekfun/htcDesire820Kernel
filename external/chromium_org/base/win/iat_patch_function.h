// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_WIN_IAT_PATCH_FUNCTION_H_
#define BASE_WIN_IAT_PATCH_FUNCTION_H_

#include <windows.h>

#include "base/base_export.h"
#include "base/basictypes.h"

namespace base {
namespace win {

class BASE_EXPORT IATPatchFunction {
 public:
  IATPatchFunction();
  ~IATPatchFunction();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  DWORD Patch(const wchar_t* module,
              const char* imported_from_module,
              const char* function_name,
              void* new_function);

  
  
  
  
  DWORD Unpatch();

  bool is_patched() const {
    return (NULL != intercept_function_);
  }

 private:
  HMODULE module_handle_;
  void* intercept_function_;
  void* original_function_;
  IMAGE_THUNK_DATA* iat_thunk_;

  DISALLOW_COPY_AND_ASSIGN(IATPatchFunction);
};

}  
}  

#endif  
