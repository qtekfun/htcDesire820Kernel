// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_CROS_SETTINGS_PROVIDER_H_
#define CHROME_BROWSER_CHROMEOS_CROS_SETTINGS_PROVIDER_H_

#include <string>

class Value;

namespace chromeos {

class CrosSettingsProvider {
 public:
  virtual ~CrosSettingsProvider() {}

  
  
  void Set(const std::string& path, Value* in_value);

  
  
  virtual bool Get(const std::string& path, Value** out_value) const = 0;

  
  virtual bool HandlesSetting(const std::string& path) = 0;

 private:
  
  virtual void DoSet(const std::string& path, Value* in_value) = 0;
};

}  

#endif  
