// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_WIN_OBJECT_WATCHER_H_
#define BASE_WIN_OBJECT_WATCHER_H_
#pragma once

#include <windows.h>

#include "base/base_api.h"
#include "base/message_loop.h"

namespace base {
namespace win {

class BASE_API ObjectWatcher : public MessageLoop::DestructionObserver {
 public:
  class Delegate {
   public:
    virtual ~Delegate() {}
    
    
    virtual void OnObjectSignaled(HANDLE object) = 0;
  };

  ObjectWatcher();
  ~ObjectWatcher();

  
  
  
  
  
  
  bool StartWatching(HANDLE object, Delegate* delegate);

  
  
  
  
  
  
  bool StopWatching();

  
  
  HANDLE GetWatchedObject();

 private:
  
  static void CALLBACK DoneWaiting(void* param, BOOLEAN timed_out);

  
  virtual void WillDestroyCurrentMessageLoop();

  
  struct Watch;
  Watch* watch_;

  DISALLOW_COPY_AND_ASSIGN(ObjectWatcher);
};

}  
}  

#endif  
