// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_METRICS_CROS_SETTINGS_PROVIDER_H_
#define CHROME_BROWSER_CHROMEOS_METRICS_CROS_SETTINGS_PROVIDER_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "chrome/browser/chromeos/cros_settings_provider.h"

namespace chromeos {

class MetricsCrosSettingsProvider : public CrosSettingsProvider {
 public:
  MetricsCrosSettingsProvider() {}

  
  
  
  static bool SetMetricsStatus(bool enabled);
  static bool GetMetricsStatus();

 private:
  
  virtual void DoSet(const std::string& path, Value* value);
  virtual bool Get(const std::string& path, Value** value) const;
  virtual bool HandlesSetting(const std::string& path);

  DISALLOW_COPY_AND_ASSIGN(MetricsCrosSettingsProvider);
};

};  

#endif  
