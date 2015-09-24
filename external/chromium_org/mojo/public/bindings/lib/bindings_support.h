// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_BINDINGS_LIB_BINDINGS_SUPPORT_H_
#define MOJO_PUBLIC_BINDINGS_LIB_BINDINGS_SUPPORT_H_

#include "mojo/public/system/core_cpp.h"

namespace mojo {
class Buffer;

class BindingsSupport {
 public:
  class AsyncWaitCallback {
   public:
    virtual ~AsyncWaitCallback() {}
    virtual void OnHandleReady(MojoResult result) = 0;
  };

  typedef void* AsyncWaitID;

  static void Set(BindingsSupport* support);
  static BindingsSupport* Get();

  
  
  virtual Buffer* GetCurrentBuffer() = 0;
  virtual Buffer* SetCurrentBuffer(Buffer* buf) = 0;

  
  
  
  
  virtual AsyncWaitID AsyncWait(const Handle& handle,
                                MojoWaitFlags flags,
                                AsyncWaitCallback* callback) = 0;

  
  
  
  virtual void CancelWait(AsyncWaitID id) = 0;

 protected:
  virtual ~BindingsSupport() {}
};

}  

#endif  
