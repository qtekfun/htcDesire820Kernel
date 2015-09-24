// Copyright 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_BASE_COMPLETION_EVENT_H_
#define CC_BASE_COMPLETION_EVENT_H_

#include "base/logging.h"
#include "base/synchronization/waitable_event.h"
#include "base/threading/thread_restrictions.h"

namespace cc {

class CompletionEvent {
 public:
  CompletionEvent()
      : event_(false , false ) {
#ifndef NDEBUG
    waited_ = false;
    signaled_ = false;
#endif
  }

  ~CompletionEvent() {
#ifndef NDEBUG
    DCHECK(waited_);
    DCHECK(signaled_);
#endif
  }

  void Wait() {
#ifndef NDEBUG
    DCHECK(!waited_);
    waited_ = true;
#endif
    base::ThreadRestrictions::ScopedAllowWait allow_wait;
    event_.Wait();
  }

  void Signal() {
#ifndef NDEBUG
    DCHECK(!signaled_);
    signaled_ = true;
#endif
    event_.Signal();
  }

 private:
  base::WaitableEvent event_;
#ifndef NDEBUG
  
  bool waited_;
  bool signaled_;
#endif
};

}  

#endif  
