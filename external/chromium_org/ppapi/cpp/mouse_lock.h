// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_MOUSE_LOCK_H_
#define PPAPI_CPP_MOUSE_LOCK_H_

#include "ppapi/c/pp_stdint.h"
#include "ppapi/cpp/instance_handle.h"


namespace pp {

class CompletionCallback;
class Instance;

class MouseLock {
 public:
  
  
  
  
  explicit MouseLock(Instance* instance);

  
  virtual ~MouseLock();

  
  virtual void MouseLockLost() = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int32_t LockMouse(const CompletionCallback& cc);

  
  
  
  
  void UnlockMouse();

 private:
  InstanceHandle associated_instance_;
};

}  

#endif  
