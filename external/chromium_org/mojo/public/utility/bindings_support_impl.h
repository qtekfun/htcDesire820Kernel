// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_UTILITY_BINDINGS_SUPPORT_IMPL_H_
#define MOJO_PUBLIC_UTILITY_BINDINGS_SUPPORT_IMPL_H_

#include "mojo/public/bindings/lib/bindings_support.h"
#include "mojo/public/system/macros.h"

namespace mojo {
namespace utility {
namespace internal {

class BindingsSupportImpl : public BindingsSupport {
 public:
  BindingsSupportImpl();
  virtual ~BindingsSupportImpl();

  
  
  static void SetUp();

  
  static void TearDown();

  
  virtual Buffer* GetCurrentBuffer() MOJO_OVERRIDE;
  virtual Buffer* SetCurrentBuffer(Buffer* buf) MOJO_OVERRIDE;
  virtual AsyncWaitID AsyncWait(const Handle& handle,
                                MojoWaitFlags flags,
                                AsyncWaitCallback* callback) MOJO_OVERRIDE;
  virtual void CancelWait(AsyncWaitID async_wait_id) MOJO_OVERRIDE;

 private:
  MOJO_DISALLOW_COPY_AND_ASSIGN(BindingsSupportImpl);
};

}  
}  
}  

#endif  
