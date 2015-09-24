// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_PROFILER_CONTROLLER_IMPL_H_
#define CONTENT_BROWSER_PROFILER_CONTROLLER_IMPL_H_

#include "base/memory/singleton.h"
#include "base/process/process.h"
#include "content/common/content_export.h"
#include "content/public/browser/profiler_controller.h"
#include "content/public/common/process_type.h"

namespace tracked_objects {
struct ProcessDataSnapshot;
}

namespace content {

class ProfilerControllerImpl : public ProfilerController {
 public:
  static ProfilerControllerImpl* GetInstance();

  
  
  ProfilerControllerImpl();
  virtual ~ProfilerControllerImpl();

  
  
  
  
  
  void OnPendingProcesses(int sequence_number, int pending_processes, bool end);

  
  
  void OnProfilerDataCollected(
      int sequence_number,
      const tracked_objects::ProcessDataSnapshot& profiler_data,
      int process_type);

  
  virtual void Register(ProfilerSubscriber* subscriber) OVERRIDE;
  virtual void Unregister(const ProfilerSubscriber* subscriber) OVERRIDE;
  virtual void GetProfilerData(int sequence_number) OVERRIDE;

 private:
  friend struct DefaultSingletonTraits<ProfilerControllerImpl>;

  
  void GetProfilerDataFromChildProcesses(int sequence_number);

  ProfilerSubscriber* subscriber_;

  DISALLOW_COPY_AND_ASSIGN(ProfilerControllerImpl);
};

}  

#endif  
