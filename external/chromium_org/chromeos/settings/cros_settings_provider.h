// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_SETTINGS_CROS_SETTINGS_PROVIDER_H_
#define CHROMEOS_SETTINGS_CROS_SETTINGS_PROVIDER_H_

#include <string>

#include "base/callback.h"
#include "chromeos/chromeos_export.h"

namespace base {
class Value;
}

namespace chromeos {

class CHROMEOS_EXPORT CrosSettingsProvider {
 public:
  
  
  typedef base::Callback<void(const std::string&)> NotifyObserversCallback;

  
  enum TrustedStatus {
    
    
    TRUSTED,
    
    
    TEMPORARILY_UNTRUSTED,
    
    
    
    PERMANENTLY_UNTRUSTED,
  };

  
  
  explicit CrosSettingsProvider(const NotifyObserversCallback& notify_cb);
  virtual ~CrosSettingsProvider();

  
  void Set(const std::string& path, const base::Value& in_value);

  
  virtual const base::Value* Get(const std::string& path) const = 0;

  
  
  
  
  
  
  virtual TrustedStatus PrepareTrustedValues(
      const base::Closure& callback) = 0;

  
  virtual bool HandlesSetting(const std::string& path) const = 0;

  void SetNotifyObserversCallback(const NotifyObserversCallback& notify_cb);

 protected:
  
  void NotifyObservers(const std::string& path);

 private:
  
  virtual void DoSet(const std::string& path,
                     const base::Value& in_value) = 0;

  
  NotifyObserversCallback notify_cb_;
};

}  

#endif  
