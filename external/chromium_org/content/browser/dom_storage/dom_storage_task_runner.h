// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_DOM_STORAGE_DOM_STORAGE_TASK_RUNNER_
#define CONTENT_BROWSER_DOM_STORAGE_DOM_STORAGE_TASK_RUNNER_

#include "base/memory/ref_counted.h"
#include "base/sequenced_task_runner.h"
#include "base/threading/sequenced_worker_pool.h"
#include "base/time/time.h"
#include "content/common/content_export.h"

namespace base {
class MessageLoopProxy;
}

namespace content {

class CONTENT_EXPORT DOMStorageTaskRunner
    : public base::TaskRunner {
 public:
  enum SequenceID {
    PRIMARY_SEQUENCE,
    COMMIT_SEQUENCE
  };

  
  
  virtual bool PostDelayedTask(
      const tracked_objects::Location& from_here,
      const base::Closure& task,
      base::TimeDelta delay) = 0;

  
  virtual bool PostShutdownBlockingTask(
      const tracked_objects::Location& from_here,
      SequenceID sequence_id,
      const base::Closure& task) = 0;

  
  
  virtual bool RunsTasksOnCurrentThread() const OVERRIDE;

  
  virtual bool IsRunningOnSequence(SequenceID sequence_id) const = 0;
  bool IsRunningOnPrimarySequence() const {
    return IsRunningOnSequence(PRIMARY_SEQUENCE);
  }
  bool IsRunningOnCommitSequence() const {
    return IsRunningOnSequence(COMMIT_SEQUENCE);
  }

 protected:
  virtual ~DOMStorageTaskRunner() {}
};

class CONTENT_EXPORT DOMStorageWorkerPoolTaskRunner :
      public DOMStorageTaskRunner {
 public:
  DOMStorageWorkerPoolTaskRunner(
      base::SequencedWorkerPool* sequenced_worker_pool,
      base::SequencedWorkerPool::SequenceToken primary_sequence_token,
      base::SequencedWorkerPool::SequenceToken commit_sequence_token,
      base::MessageLoopProxy* delayed_task_loop);

  virtual bool PostDelayedTask(
      const tracked_objects::Location& from_here,
      const base::Closure& task,
      base::TimeDelta delay) OVERRIDE;

  virtual bool PostShutdownBlockingTask(
      const tracked_objects::Location& from_here,
      SequenceID sequence_id,
      const base::Closure& task) OVERRIDE;

  virtual bool IsRunningOnSequence(SequenceID sequence_id) const OVERRIDE;

 protected:
  virtual ~DOMStorageWorkerPoolTaskRunner();

 private:

  base::SequencedWorkerPool::SequenceToken IDtoToken(SequenceID id) const;

  const scoped_refptr<base::MessageLoopProxy> message_loop_;
  const scoped_refptr<base::SequencedWorkerPool> sequenced_worker_pool_;
  base::SequencedWorkerPool::SequenceToken primary_sequence_token_;
  base::SequencedWorkerPool::SequenceToken commit_sequence_token_;
};

class CONTENT_EXPORT MockDOMStorageTaskRunner :
      public DOMStorageTaskRunner {
 public:
  explicit MockDOMStorageTaskRunner(base::MessageLoopProxy* message_loop);

  virtual bool PostDelayedTask(
      const tracked_objects::Location& from_here,
      const base::Closure& task,
      base::TimeDelta delay) OVERRIDE;

  virtual bool PostShutdownBlockingTask(
      const tracked_objects::Location& from_here,
      SequenceID sequence_id,
      const base::Closure& task) OVERRIDE;

  virtual bool IsRunningOnSequence(SequenceID sequence_id) const OVERRIDE;

 protected:
  virtual ~MockDOMStorageTaskRunner();

 private:
  const scoped_refptr<base::MessageLoopProxy> message_loop_;
};

}  

#endif  
