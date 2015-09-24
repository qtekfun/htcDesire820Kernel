// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_GN_SCHEDULER_H_
#define TOOLS_GN_SCHEDULER_H_

#include "base/atomic_ref_count.h"
#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/message_loop/message_loop.h"
#include "base/run_loop.h"
#include "base/synchronization/lock.h"
#include "base/threading/sequenced_worker_pool.h"
#include "tools/gn/input_file_manager.h"

class Target;

class Scheduler {
 public:
  Scheduler();
  ~Scheduler();

  bool Run();

  base::MessageLoop* main_loop() { return &main_loop_; }
  base::SequencedWorkerPool* pool() { return pool_; }

  InputFileManager* input_file_manager() { return input_file_manager_; }

  bool verbose_logging() const { return verbose_logging_; }
  void set_verbose_logging(bool v) { verbose_logging_ = v; }

  
  bool is_failed() const { return is_failed_; }

  void Log(const std::string& verb, const std::string& msg);
  void FailWithError(const Err& err);

  void ScheduleWork(const base::Closure& work);

  void Shutdown();

  
  
  
  
  
  void AddGenDependency(const base::FilePath& file);
  std::vector<base::FilePath> GetGenDependencies() const;

  
  
  void IncrementWorkCount();
  void DecrementWorkCount();

 private:
  void LogOnMainThread(const std::string& verb, const std::string& msg);
  void FailWithErrorOnMainThread(const Err& err);

  void DoTargetFileWrite(const Target* target);

  void DoWork(const base::Closure& closure);

  void OnComplete();

  base::MessageLoop main_loop_;
  scoped_refptr<base::SequencedWorkerPool> pool_;

  scoped_refptr<InputFileManager> input_file_manager_;

  base::RunLoop runner_;

  bool verbose_logging_;

  base::AtomicRefCount work_count_;

  mutable base::Lock lock_;
  bool is_failed_;

  
  
  
  bool has_been_shutdown_;

  
  std::vector<base::FilePath> gen_dependencies_;

  DISALLOW_COPY_AND_ASSIGN(Scheduler);
};

extern Scheduler* g_scheduler;

#endif  

