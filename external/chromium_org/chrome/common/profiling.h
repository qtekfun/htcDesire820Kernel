// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_PROFILING_H_
#define CHROME_COMMON_PROFILING_H_

#include "build/build_config.h"

#include "base/basictypes.h"
#include "base/debug/profiler.h"

class Profiling {
 public:
  
  
  static void ProcessStarted();

  
  static void Start();

  
  static void Stop();

  
  static bool BeingProfiled();

  
  static void Toggle();

 private:
  
  Profiling();

  DISALLOW_COPY_AND_ASSIGN(Profiling);
};

#endif  
