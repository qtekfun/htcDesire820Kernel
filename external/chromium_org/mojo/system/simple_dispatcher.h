// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_SYSTEM_SIMPLE_DISPATCHER_H_
#define MOJO_SYSTEM_SIMPLE_DISPATCHER_H_

#include <list>

#include "base/basictypes.h"
#include "mojo/system/dispatcher.h"
#include "mojo/system/system_impl_export.h"
#include "mojo/system/waiter_list.h"

namespace mojo {
namespace system {

class MOJO_SYSTEM_IMPL_EXPORT SimpleDispatcher : public Dispatcher {
 protected:
  SimpleDispatcher();

  friend class base::RefCountedThreadSafe<SimpleDispatcher>;
  virtual ~SimpleDispatcher();

  
  
  
  void StateChangedNoLock();

  
  
  
  
  
  virtual MojoWaitFlags SatisfiedFlagsNoLock() const = 0;
  virtual MojoWaitFlags SatisfiableFlagsNoLock() const = 0;

  
  virtual void CancelAllWaitersNoLock() OVERRIDE;
  virtual MojoResult AddWaiterImplNoLock(Waiter* waiter,
                                         MojoWaitFlags flags,
                                         MojoResult wake_result) OVERRIDE;
  virtual void RemoveWaiterImplNoLock(Waiter* waiter) OVERRIDE;

 private:
  
  WaiterList waiter_list_;

  DISALLOW_COPY_AND_ASSIGN(SimpleDispatcher);
};

}  
}  

#endif  
