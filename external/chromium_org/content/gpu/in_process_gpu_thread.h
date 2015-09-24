// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_GPU_IN_PROCESS_GPU_THREAD_H_
#define CONTENT_GPU_IN_PROCESS_GPU_THREAD_H_

#include "base/threading/thread.h"
#include "content/common/content_export.h"

namespace content {

class GpuProcess;

class InProcessGpuThread : public base::Thread {
 public:
  explicit InProcessGpuThread(const std::string& channel_id);
  virtual ~InProcessGpuThread();

 protected:
  virtual void Init() OVERRIDE;
  virtual void CleanUp() OVERRIDE;

 private:
  std::string channel_id_;
  
  GpuProcess* gpu_process_;

  DISALLOW_COPY_AND_ASSIGN(InProcessGpuThread);
};

CONTENT_EXPORT base::Thread* CreateInProcessGpuThread(
    const std::string& channel_id);

}  

#endif  
