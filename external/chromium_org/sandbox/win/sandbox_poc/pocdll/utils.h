// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SANDBOX_SANDBOX_POC_POCDLL_UTILS_H__
#define SANDBOX_SANDBOX_POC_POCDLL_UTILS_H__

#include <stdio.h>
#include <io.h>
#include "base/basictypes.h"

class HandleToFile {
 public:
  HandleToFile() {
    file_ = NULL;
  };

  
  ~HandleToFile() {
    if (file_) {
      fflush(file_);
      fclose(file_);
    }
  };

  
  
  FILE* Translate(HANDLE handle, const char *mode) {
    if (file_) {
      return  NULL;
    }

    HANDLE new_handle;
    BOOL result = ::DuplicateHandle(::GetCurrentProcess(),
                                    handle,
                                    ::GetCurrentProcess(),
                                    &new_handle,
                                    0,  
                                        
                                    FALSE,  
                                    DUPLICATE_SAME_ACCESS);

    if (!result) {
      return NULL;
    }

    int c_file_handle = _open_osfhandle(reinterpret_cast<LONG_PTR>(new_handle),
                                        0);  
    if (-1 == c_file_handle) {
      return NULL;
    }

    file_ = _fdopen(c_file_handle, mode);
    return file_;
  };
 private:
  
  FILE* file_;

  DISALLOW_COPY_AND_ASSIGN(HandleToFile);
};

#endif  
