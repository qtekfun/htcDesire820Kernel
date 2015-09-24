// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_SYSTEM_WAITER_LIST_H_
#define MOJO_SYSTEM_WAITER_LIST_H_

#include <list>

#include "base/basictypes.h"
#include "mojo/public/system/core.h"
#include "mojo/system/system_impl_export.h"

namespace mojo {
namespace system {

class Waiter;

class MOJO_SYSTEM_IMPL_EXPORT WaiterList {
 public:
  WaiterList();
  ~WaiterList();

  void AwakeWaitersForStateChange(MojoWaitFlags satisfied_flags,
                                  MojoWaitFlags satisfiable_flags);
  void CancelAllWaiters();
  void AddWaiter(Waiter* waiter, MojoWaitFlags flags, MojoResult wake_result);
  void RemoveWaiter(Waiter* waiter);

 private:
  struct WaiterInfo {
    WaiterInfo(Waiter* waiter, MojoWaitFlags flags, MojoResult wake_result)
        : waiter(waiter), flags(flags), wake_result(wake_result) {}

    Waiter* waiter;
    MojoWaitFlags flags;
    MojoResult wake_result;
  };
  typedef std::list<WaiterInfo> WaiterInfoList;

  WaiterInfoList waiters_;

  DISALLOW_COPY_AND_ASSIGN(WaiterList);
};

}  
}  

#endif  
