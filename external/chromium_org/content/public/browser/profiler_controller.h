// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_PROFILER_CONTROLLER_H_
#define CONTENT_PUBLIC_BROWSER_PROFILER_CONTROLLER_H_

#include <set>
#include <string>

#include "base/tracked_objects.h"
#include "content/common/content_export.h"


namespace base {
class DictionaryValue;
}

namespace content {

class ProfilerSubscriber;

class CONTENT_EXPORT ProfilerController {
 public:
  
  
  static ProfilerController* GetInstance();

  virtual ~ProfilerController() {}

  
  
  
  virtual void Register(ProfilerSubscriber* subscriber) = 0;

  
  
  
  virtual void Unregister(const ProfilerSubscriber* subscriber) = 0;

  
  virtual void GetProfilerData(int sequence_number) = 0;
};

}  

#endif  
