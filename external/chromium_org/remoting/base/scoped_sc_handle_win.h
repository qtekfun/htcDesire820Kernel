// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_BASE_SCOPED_SC_HANDLE_WIN_H_
#define REMOTING_BASE_SCOPED_SC_HANDLE_WIN_H_

#include <windows.h>

#include "base/win/scoped_handle.h"

namespace remoting {

class ScHandleTraits {
 public:
  typedef SC_HANDLE Handle;

  
  static bool CloseHandle(SC_HANDLE handle) {
    return ::CloseServiceHandle(handle) != FALSE;
  }

  
  static bool IsHandleValid(SC_HANDLE handle) {
    return handle != NULL;
  }

  
  static SC_HANDLE NullHandle() {
    return NULL;
  }

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(ScHandleTraits);
};

typedef base::win::GenericScopedHandle<
    ScHandleTraits, base::win::DummyVerifierTraits> ScopedScHandle;

}  

#endif  
