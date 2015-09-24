// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TASK_QUEUE_H_
#define BASE_TASK_QUEUE_H_
#pragma once

#include <deque>

#include "base/base_api.h"
#include "base/task.h"

class BASE_API TaskQueue : public Task {
 public:
  TaskQueue();
  ~TaskQueue();

  
  
  
  
  
  void Push(Task* task);

  
  void Clear();

  
  bool IsEmpty() const;

  
  
  virtual void Run();

 private:
   
   std::deque<Task*> queue_;
};

#endif  
