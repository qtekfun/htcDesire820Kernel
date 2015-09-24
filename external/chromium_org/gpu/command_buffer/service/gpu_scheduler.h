// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GPU_COMMAND_BUFFER_SERVICE_GPU_SCHEDULER_H_
#define GPU_COMMAND_BUFFER_SERVICE_GPU_SCHEDULER_H_

#include <queue>

#include "base/atomic_ref_count.h"
#include "base/atomicops.h"
#include "base/callback.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/shared_memory.h"
#include "base/memory/weak_ptr.h"
#include "gpu/command_buffer/common/command_buffer.h"
#include "gpu/command_buffer/service/cmd_buffer_engine.h"
#include "gpu/command_buffer/service/cmd_parser.h"
#include "gpu/command_buffer/service/gles2_cmd_decoder.h"
#include "gpu/gpu_export.h"

namespace gfx {
class GLFence;
}

namespace gpu {

class PreemptionFlag
    : public base::RefCountedThreadSafe<PreemptionFlag> {
 public:
  PreemptionFlag() : flag_(0) {}

  bool IsSet() { return !base::AtomicRefCountIsZero(&flag_); }
  void Set() { base::AtomicRefCountInc(&flag_); }
  void Reset() { base::subtle::NoBarrier_Store(&flag_, 0); }

 private:
  base::AtomicRefCount flag_;

  ~PreemptionFlag() {}

  friend class base::RefCountedThreadSafe<PreemptionFlag>;
};

class GPU_EXPORT GpuScheduler
    : NON_EXPORTED_BASE(public CommandBufferEngine),
      public base::SupportsWeakPtr<GpuScheduler> {
 public:
  GpuScheduler(CommandBuffer* command_buffer,
               AsyncAPIInterface* handler,
               gles2::GLES2Decoder* decoder);

  virtual ~GpuScheduler();

  void PutChanged();

  void SetPreemptByFlag(scoped_refptr<PreemptionFlag> flag) {
    preemption_flag_ = flag;
  }

  
  
  
  
  void SetScheduled(bool is_scheduled);

  
  bool IsScheduled();

  
  bool HasMoreWork();

  typedef base::Callback<void(bool )> SchedulingChangedCallback;

  
  
  void SetSchedulingChangedCallback(const SchedulingChangedCallback& callback);

  
  virtual Buffer GetSharedMemoryBuffer(int32 shm_id) OVERRIDE;
  virtual void set_token(int32 token) OVERRIDE;
  virtual bool SetGetBuffer(int32 transfer_buffer_id) OVERRIDE;
  virtual bool SetGetOffset(int32 offset) OVERRIDE;
  virtual int32 GetGetOffset() OVERRIDE;

  void SetCommandProcessedCallback(const base::Closure& callback);

  void DeferToFence(base::Closure task);

  
  
  bool PollUnscheduleFences();

  bool HasMoreIdleWork();
  void PerformIdleWork();

  CommandParser* parser() const {
    return parser_.get();
  }

  bool IsPreempted();

 private:
  
  
  void RescheduleTimeOut();

  
  
  
  CommandBuffer* command_buffer_;

  
  AsyncAPIInterface* handler_;

  
  
  
  
  gles2::GLES2Decoder* decoder_;

  
  
  scoped_ptr<CommandParser> parser_;

  
  int unscheduled_count_;

  
  
  int rescheduled_count_;

  
  
  base::WeakPtrFactory<GpuScheduler> reschedule_task_factory_;

  
  
  struct UnscheduleFence {
    UnscheduleFence(gfx::GLFence* fence, base::Closure task);
    ~UnscheduleFence();

    scoped_ptr<gfx::GLFence> fence;
    base::Time issue_time;
    base::Closure task;
  };
  std::queue<linked_ptr<UnscheduleFence> > unschedule_fences_;

  SchedulingChangedCallback scheduling_changed_callback_;
  base::Closure descheduled_callback_;
  base::Closure command_processed_callback_;

  
  scoped_refptr<PreemptionFlag> preemption_flag_;
  bool was_preempted_;

  DISALLOW_COPY_AND_ASSIGN(GpuScheduler);
};

}  

#endif  
