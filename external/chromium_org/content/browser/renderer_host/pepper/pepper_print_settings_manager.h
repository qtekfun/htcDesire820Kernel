// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_PEPPER_PEPPER_PRINT_SETTINGS_MANAGER_H_
#define CONTENT_BROWSER_RENDERER_HOST_PEPPER_PEPPER_PRINT_SETTINGS_MANAGER_H_

#include "base/bind.h"
#include "base/compiler_specific.h"
#include "content/common/content_export.h"
#include "ppapi/c/dev/pp_print_settings_dev.h"

namespace content {

class CONTENT_EXPORT PepperPrintSettingsManager {
 public:
  typedef std::pair<PP_PrintSettings_Dev, int32_t> Result;
  typedef base::Callback<void(Result)> Callback;

  
  
  
  
  virtual void GetDefaultPrintSettings(Callback callback) = 0;

  virtual ~PepperPrintSettingsManager() {}
};

class CONTENT_EXPORT PepperPrintSettingsManagerImpl
    : public PepperPrintSettingsManager {
 public:
  PepperPrintSettingsManagerImpl() {}
  virtual ~PepperPrintSettingsManagerImpl() {}

  
  virtual void GetDefaultPrintSettings(
      PepperPrintSettingsManager::Callback callback) OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(PepperPrintSettingsManagerImpl);
};

}  

#endif  
