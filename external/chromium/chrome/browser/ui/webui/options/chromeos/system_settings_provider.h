// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_CHROMEOS_SYSTEM_SETTINGS_PROVIDER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_CHROMEOS_SYSTEM_SETTINGS_PROVIDER_H_

#include <vector>

#include "base/string16.h"
#include "chrome/browser/chromeos/cros_settings_provider.h"
#include "chrome/browser/chromeos/system_access.h"
#include "third_party/icu/public/i18n/unicode/timezone.h"

class Value;
class ListValue;

namespace chromeos {

class SystemSettingsProvider : public CrosSettingsProvider,
                               public SystemAccess::Observer {
 public:
  SystemSettingsProvider();
  virtual ~SystemSettingsProvider();

  
  virtual bool Get(const std::string& path, Value** out_value) const;
  virtual bool HandlesSetting(const std::string& path);

  
  virtual void TimezoneChanged(const icu::TimeZone& timezone);

  
  ListValue* GetTimezoneList();

 private:
  
  virtual void DoSet(const std::string& path, Value* in_value);

  
  static string16 GetTimezoneName(const icu::TimeZone& timezone);

  
  static string16 GetTimezoneID(const icu::TimeZone& timezone);

  
  const icu::TimeZone* GetTimezone(const string16& timezone_id);

  
  
  
  
  
  
  
  string16 GetKnownTimezoneID(const icu::TimeZone& timezone) const;

  
  std::vector<icu::TimeZone*> timezones_;

  DISALLOW_COPY_AND_ASSIGN(SystemSettingsProvider);
};

}  

#endif  
