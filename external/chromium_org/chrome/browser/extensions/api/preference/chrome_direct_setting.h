// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_PREFERENCE_CHROME_DIRECT_SETTING_H__
#define CHROME_BROWSER_EXTENSIONS_API_PREFERENCE_CHROME_DIRECT_SETTING_H__

#include "base/lazy_instance.h"
#include "chrome/browser/extensions/chrome_extension_function.h"

class PrefService;

namespace extensions {
namespace chromedirectsetting {

class DirectSettingFunctionBase : public ChromeSyncExtensionFunction {
 protected:
  DirectSettingFunctionBase();
  virtual ~DirectSettingFunctionBase();

  
  PrefService* GetPrefService();

  
  bool IsCalledFromComponentExtension();

 private:
  DISALLOW_COPY_AND_ASSIGN(DirectSettingFunctionBase);
};

class GetDirectSettingFunction : public DirectSettingFunctionBase {
 public:
  DECLARE_EXTENSION_FUNCTION("types.private.ChromeDirectSetting.get",
                             TYPES_PRIVATE_CHROMEDIRECTSETTING_GET)

  GetDirectSettingFunction();

 protected:
  
  virtual bool RunImpl() OVERRIDE;

 private:
  virtual ~GetDirectSettingFunction();
  DISALLOW_COPY_AND_ASSIGN(GetDirectSettingFunction);
};

class SetDirectSettingFunction : public DirectSettingFunctionBase {
 public:
  DECLARE_EXTENSION_FUNCTION("types.private.ChromeDirectSetting.set",
                             TYPES_PRIVATE_CHROMEDIRECTSETTING_SET)

  SetDirectSettingFunction();

 protected:
  
  virtual bool RunImpl() OVERRIDE;

 private:
  virtual ~SetDirectSettingFunction();
  DISALLOW_COPY_AND_ASSIGN(SetDirectSettingFunction);
};

class ClearDirectSettingFunction : public DirectSettingFunctionBase {
 public:
  DECLARE_EXTENSION_FUNCTION("types.private.ChromeDirectSetting.clear",
                             TYPES_PRIVATE_CHROMEDIRECTSETTING_CLEAR)

  ClearDirectSettingFunction();

 protected:
  
  virtual bool RunImpl() OVERRIDE;

 private:
  virtual ~ClearDirectSettingFunction();
  DISALLOW_COPY_AND_ASSIGN(ClearDirectSettingFunction);
};

}  
}  

#endif  

