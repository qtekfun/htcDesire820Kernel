// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_POWER_SAVE_BLOCKER_H_
#define CONTENT_PUBLIC_BROWSER_POWER_SAVE_BLOCKER_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "content/common/content_export.h"

namespace content {

class CONTENT_EXPORT PowerSaveBlocker {
 public:
  enum PowerSaveBlockerType {
    
    
    
    
    
    kPowerSaveBlockPreventAppSuspension,

    
    
    
    
    kPowerSaveBlockPreventDisplaySleep,
  };

  virtual ~PowerSaveBlocker() = 0;

  
  
  
  static scoped_ptr<PowerSaveBlocker> Create(PowerSaveBlockerType type,
                                             const std::string& reason);
};

}  

#endif  
