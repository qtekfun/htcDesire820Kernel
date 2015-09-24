// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_SCOPED_LOOP_OBSERVER_H_
#define MEDIA_AUDIO_SCOPED_LOOP_OBSERVER_H_

#include "base/memory/ref_counted.h"
#include "base/message_loop/message_loop.h"
#include "base/message_loop/message_loop_proxy.h"

namespace base {
class WaitableEvent;
}

namespace media {

class ScopedLoopObserver
    : public base::MessageLoop::DestructionObserver {
 public:
  explicit ScopedLoopObserver(
      const scoped_refptr<base::MessageLoopProxy>& message_loop);

 protected:
  virtual ~ScopedLoopObserver();

  
  const scoped_refptr<base::MessageLoopProxy>& message_loop() { return loop_; }

 private:
  
  
  void ObserveLoopDestruction(bool enable, base::WaitableEvent* done);

  
  
  scoped_refptr<base::MessageLoopProxy> loop_;

  DISALLOW_COPY_AND_ASSIGN(ScopedLoopObserver);
};

}  

#endif  
