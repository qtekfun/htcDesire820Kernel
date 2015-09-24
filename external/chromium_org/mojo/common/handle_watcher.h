// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_COMMON_HANDLE_WATCHER_H_
#define MOJO_COMMON_HANDLE_WATCHER_H_

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/memory/scoped_ptr.h"
#include "mojo/common/mojo_common_export.h"
#include "mojo/public/system/core_cpp.h"

namespace base {
class Thread;
class TickClock;
class TimeTicks;
}

namespace mojo {
namespace common {
namespace test {
class HandleWatcherTest;
}

class MOJO_COMMON_EXPORT HandleWatcher {
 public:
  HandleWatcher();
  ~HandleWatcher();

  
  
  
  
  
  void Start(const Handle& handle,
             MojoWaitFlags wait_flags,
             MojoDeadline deadline,
             const base::Callback<void(MojoResult)>& callback);

  
  void Stop();

  
  static base::TimeTicks NowTicks();

  
  static base::TimeTicks MojoDeadlineToTimeTicks(MojoDeadline deadline);

 private:
  friend class test::HandleWatcherTest;
  struct StartState;

  
  void OnHandleReady(MojoResult result);

  
  scoped_ptr<StartState> start_state_;

  
  static base::TickClock* tick_clock_;

  DISALLOW_COPY_AND_ASSIGN(HandleWatcher);
};

}  
}  

#endif  
