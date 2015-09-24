// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_THREADING_SEQUENCED_WORKER_POOL_UNITTEST_H_
#define BASE_THREADING_SEQUENCED_WORKER_POOL_UNITTEST_H_

#include <cstddef>
#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/synchronization/lock.h"
#include "base/threading/sequenced_worker_pool.h"

namespace base {

class MessageLoop;

class SequencedWorkerPoolOwner : public SequencedWorkerPool::TestingObserver {
 public:
  SequencedWorkerPoolOwner(size_t max_threads,
                           const std::string& thread_name_prefix);

  virtual ~SequencedWorkerPoolOwner();

  
  const scoped_refptr<SequencedWorkerPool>& pool();

  
  void SetWillWaitForShutdownCallback(const Closure& callback);

  int has_work_call_count() const;

 private:
  
  virtual void OnHasWork() OVERRIDE;
  virtual void WillWaitForShutdown() OVERRIDE;
  virtual void OnDestruct() OVERRIDE;

  MessageLoop* const constructor_message_loop_;
  scoped_refptr<SequencedWorkerPool> pool_;
  Closure will_wait_for_shutdown_callback_;

  mutable Lock has_work_lock_;
  int has_work_call_count_;

  DISALLOW_COPY_AND_ASSIGN(SequencedWorkerPoolOwner);
};

}  

#endif  
