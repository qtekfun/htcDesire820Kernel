// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_POWER_SAVE_BLOCKER_IMPL_H_
#define CONTENT_BROWSER_POWER_SAVE_BLOCKER_IMPL_H_

#include "base/memory/ref_counted.h"
#include "content/public/browser/power_save_blocker.h"
#include "ui/gfx/native_widget_types.h"

namespace content {

class PowerSaveBlockerImpl : public PowerSaveBlocker {
 public:
  PowerSaveBlockerImpl(PowerSaveBlockerType type, const std::string& reason);
  virtual ~PowerSaveBlockerImpl();

#if defined(OS_ANDROID)
  
  
  
  void InitDisplaySleepBlocker(gfx::NativeView view_android);
#endif

 private:
  class Delegate;

  
  
  
  
  
  
  
  
  
  
  scoped_refptr<Delegate> delegate_;

  DISALLOW_COPY_AND_ASSIGN(PowerSaveBlockerImpl);
};

}  

#endif  
