// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RLZ_WIN_LIB_PROCESS_INFO_H_
#define RLZ_WIN_LIB_PROCESS_INFO_H_

#include "base/basictypes.h"

namespace rlz_lib {

class ProcessInfo {
 public:
  enum IntegrityLevel {
    INTEGRITY_UNKNOWN,
    LOW_INTEGRITY,
    MEDIUM_INTEGRITY,
    HIGH_INTEGRITY,
  };

  
  static bool IsRunningAsSystem();
  static bool HasAdminRights();  

 private:
  DISALLOW_COPY_AND_ASSIGN(ProcessInfo);
};  
};  

#endif  
