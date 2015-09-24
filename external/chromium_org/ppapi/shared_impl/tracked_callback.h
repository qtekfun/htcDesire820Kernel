// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_SHARED_IMPL_TRACKED_CALLBACK_H_
#define PPAPI_SHARED_IMPL_TRACKED_CALLBACK_H_

#include <map>
#include <set>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/condition_variable.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/shared_impl/ppapi_shared_export.h"
#include "ppapi/shared_impl/ppb_message_loop_shared.h"

namespace ppapi {

class CallbackTracker;
class MessageLoopShared;
class Resource;

namespace thunk {
namespace subtle {
class EnterBase;
}
}

class PPAPI_SHARED_EXPORT TrackedCallback
    : public base::RefCountedThreadSafe<TrackedCallback> {
 public:
  
  
  
  TrackedCallback(Resource* resource, const PP_CompletionCallback& callback);

  
  
  void Abort();
  void PostAbort();

  
  
  
  
  
  
  
  void Run(int32_t result);
  
  
  
  void PostRun(int32_t result);

  
  
  
  
  
  typedef base::Callback<int32_t(int32_t )> CompletionTask;

  
  
  void set_completion_task(const CompletionTask& completion_task);

  
  
  PP_Resource resource_id() const { return resource_id_; }

  
  bool completed() const { return completed_; }

  
  
  
  bool aborted() const { return aborted_; }

  
  bool is_blocking() {
    return !callback_.func;
  }

  MessageLoopShared* target_loop() const {
    return target_loop_.get();
  }

  
  
  
  
  
  
  
  
  static bool IsPending(const scoped_refptr<TrackedCallback>& callback);

  
  
  static bool IsScheduledToRun(const scoped_refptr<TrackedCallback>& callback);

 protected:
  bool is_required() {
    return (callback_.func &&
            !(callback_.flags & PP_COMPLETIONCALLBACK_FLAG_OPTIONAL));
  }
  bool is_optional() {
    return (callback_.func &&
            (callback_.flags & PP_COMPLETIONCALLBACK_FLAG_OPTIONAL));
  }
  bool has_null_target_loop() const { return target_loop_.get() == NULL; }

 private:
  
  
  
  friend class ppapi::thunk::subtle::EnterBase;

  
  
  int32_t BlockUntilComplete();

  
  
  
  void MarkAsCompleted();

  
  friend class base::RefCountedThreadSafe<TrackedCallback>;
  virtual ~TrackedCallback();

  
  
  bool is_scheduled_;

  scoped_refptr<CallbackTracker> tracker_;
  PP_Resource resource_id_;
  bool completed_;
  bool aborted_;
  PP_CompletionCallback callback_;

  
  CompletionTask completion_task_;

  
  
  scoped_refptr<MessageLoopShared> target_loop_;

  int32_t result_for_blocked_callback_;
  
  
  
  scoped_ptr<base::ConditionVariable> operation_completed_condvar_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(TrackedCallback);
};

}  

#endif  
