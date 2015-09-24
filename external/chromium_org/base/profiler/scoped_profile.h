// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_PROFILER_SCOPED_PROFILE_H_
#define BASE_PROFILER_SCOPED_PROFILE_H_


#include "base/base_export.h"
#include "base/location.h"
#include "base/profiler/tracked_time.h"

#if defined(GOOGLE_CHROME_BUILD)

#define TRACK_RUN_IN_THIS_SCOPED_REGION_FOR_DEVELOPER_BUILDS(scope_name)       \
    ((void)0)

#else

#define TRACK_RUN_IN_THIS_SCOPED_REGION_FOR_DEVELOPER_BUILDS(scope_name)       \
    ::tracked_objects::ScopedProfile  LINE_BASED_VARIABLE_NAME_FOR_PROFILING(  \
        FROM_HERE_WITH_EXPLICIT_FUNCTION(#scope_name))

#endif



#define PASTE_LINE_NUMBER_ON_NAME(name, line) name##line

#define LINE_BASED_VARIABLE_NAME_FOR_PROFILING                                 \
    PASTE_LINE_NUMBER_ON_NAME(some_profiler_variable_, __LINE__)

#define TRACK_RUN_IN_IPC_HANDLER(dispatch_function_name)                       \
    ::tracked_objects::ScopedProfile some_tracking_variable_name(              \
        FROM_HERE_WITH_EXPLICIT_FUNCTION(#dispatch_function_name))


namespace tracked_objects {
class Births;

class BASE_EXPORT ScopedProfile {
 public:
  explicit ScopedProfile(const Location& location);
  ~ScopedProfile();

  
  void StopClockAndTally();

 private:
  Births* birth_;  
  const TrackedTime start_of_run_;

  DISALLOW_COPY_AND_ASSIGN(ScopedProfile);
};

}  

#endif   
