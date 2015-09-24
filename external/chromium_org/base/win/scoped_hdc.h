// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_WIN_SCOPED_HDC_H_
#define BASE_WIN_SCOPED_HDC_H_

#include <windows.h>

#include "base/basictypes.h"
#include "base/logging.h"
#include "base/win/scoped_handle.h"

namespace base {
namespace win {

class ScopedGetDC {
 public:
  explicit ScopedGetDC(HWND hwnd)
      : hwnd_(hwnd),
        hdc_(GetDC(hwnd)) {
    if (hwnd_) {
      DCHECK(IsWindow(hwnd_));
      DCHECK(hdc_);
    } else {
      
      
      
      CHECK(hdc_);
    }
  }

  ~ScopedGetDC() {
    if (hdc_)
      ReleaseDC(hwnd_, hdc_);
  }

  operator HDC() { return hdc_; }

 private:
  HWND hwnd_;
  HDC hdc_;

  DISALLOW_COPY_AND_ASSIGN(ScopedGetDC);
};

class CreateDCTraits {
 public:
  typedef HDC Handle;

  static bool CloseHandle(HDC handle) {
    return ::DeleteDC(handle) != FALSE;
  }

  static bool IsHandleValid(HDC handle) {
    return handle != NULL;
  }

  static HDC NullHandle() {
    return NULL;
  }

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(CreateDCTraits);
};

typedef GenericScopedHandle<CreateDCTraits, VerifierTraits> ScopedCreateDC;

}  
}  

#endif  
