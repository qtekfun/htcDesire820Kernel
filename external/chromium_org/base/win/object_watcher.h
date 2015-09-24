// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_WIN_OBJECT_WATCHER_H_
#define BASE_WIN_OBJECT_WATCHER_H_

#include <windows.h>

#include "base/base_export.h"
#include "base/callback.h"
#include "base/memory/weak_ptr.h"
#include "base/message_loop/message_loop.h"

namespace base {
namespace win {

class BASE_EXPORT ObjectWatcher : public MessageLoop::DestructionObserver {
 public:
  class BASE_EXPORT Delegate {
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

  void Signal(Delegate* delegate);

  
  virtual void WillDestroyCurrentMessageLoop();

  
  WeakPtrFactory<ObjectWatcher> weak_factory_;
  Closure callback_;
  HANDLE object_;             
  HANDLE wait_object_;        
  MessageLoop* origin_loop_;  

  DISALLOW_COPY_AND_ASSIGN(ObjectWatcher);
};

}  
}  

#endif  
