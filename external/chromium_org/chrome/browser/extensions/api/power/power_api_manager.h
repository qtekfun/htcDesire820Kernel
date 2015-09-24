// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_POWER_POWER_API_MANAGER_H_
#define CHROME_BROWSER_EXTENSIONS_API_POWER_POWER_API_MANAGER_H_

#include <map>
#include <string>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/singleton.h"
#include "chrome/common/extensions/api/power.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/power_save_blocker.h"

namespace extensions {

class PowerApiManager : public content::NotificationObserver {
 public:
  typedef base::Callback<scoped_ptr<content::PowerSaveBlocker>(
      content::PowerSaveBlocker::PowerSaveBlockerType,
      const std::string&)> CreateBlockerFunction;

  static PowerApiManager* GetInstance();

  
  
  void AddRequest(const std::string& extension_id, api::power::Level level);

  
  
  void RemoveRequest(const std::string& extension_id);

  
  
  void SetCreateBlockerFunctionForTesting(CreateBlockerFunction function);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  friend struct DefaultSingletonTraits<PowerApiManager>;

  PowerApiManager();
  virtual ~PowerApiManager();

  
  
  void UpdatePowerSaveBlocker();

  content::NotificationRegistrar registrar_;

  
  
  
  CreateBlockerFunction create_blocker_function_;

  scoped_ptr<content::PowerSaveBlocker> power_save_blocker_;

  
  
  api::power::Level current_level_;

  
  
  typedef std::map<std::string, api::power::Level> ExtensionLevelMap;
  ExtensionLevelMap extension_levels_;

  DISALLOW_COPY_AND_ASSIGN(PowerApiManager);
};

}  

#endif  
