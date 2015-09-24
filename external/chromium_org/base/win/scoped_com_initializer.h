// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_WIN_SCOPED_COM_INITIALIZER_H_
#define BASE_WIN_SCOPED_COM_INITIALIZER_H_

#include <objbase.h>

#include "base/basictypes.h"
#include "base/logging.h"
#include "build/build_config.h"

namespace base {
namespace win {

class ScopedCOMInitializer {
 public:
  
  enum SelectMTA { kMTA };

  
  ScopedCOMInitializer() {
    Initialize(COINIT_APARTMENTTHREADED);
  }

  
  explicit ScopedCOMInitializer(SelectMTA mta) {
    Initialize(COINIT_MULTITHREADED);
  }

  ~ScopedCOMInitializer() {
#ifndef NDEBUG
    
    DCHECK_EQ(GetCurrentThreadId(), thread_id_);
#endif
    if (succeeded())
      CoUninitialize();
  }

  bool succeeded() const { return SUCCEEDED(hr_); }

 private:
  void Initialize(COINIT init) {
#ifndef NDEBUG
    thread_id_ = GetCurrentThreadId();
#endif
    hr_ = CoInitializeEx(NULL, init);
#ifndef NDEBUG
    if (hr_ == S_FALSE)
      LOG(ERROR) << "Multiple CoInitialize() calls for thread " << thread_id_;
    else
      DCHECK_NE(RPC_E_CHANGED_MODE, hr_) << "Invalid COM thread model change";
#endif
  }

  HRESULT hr_;
#ifndef NDEBUG
  
  
  
  
  DWORD thread_id_;
#endif

  DISALLOW_COPY_AND_ASSIGN(ScopedCOMInitializer);
};

}  
}  

#endif  
