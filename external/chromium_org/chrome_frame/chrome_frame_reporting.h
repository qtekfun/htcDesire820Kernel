// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_CHROME_FRAME_REPORTING_H_
#define CHROME_FRAME_CHROME_FRAME_REPORTING_H_

#include "chrome_frame/scoped_initialization_manager.h"

namespace chrome_frame {

class CrashReportingTraits {
 public:
  static void Initialize();
  static void Shutdown();
};

typedef ScopedInitializationManager<CrashReportingTraits> ScopedCrashReporting;

}  

#endif  
