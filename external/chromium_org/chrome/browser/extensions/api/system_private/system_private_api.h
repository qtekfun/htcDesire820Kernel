// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_EXTENSIONS_API_SYSTEM_PRIVATE_SYSTEM_PRIVATE_API_H_
#define CHROME_BROWSER_EXTENSIONS_API_SYSTEM_PRIVATE_SYSTEM_PRIVATE_API_H_

#include "chrome/browser/extensions/chrome_extension_function.h"

namespace extensions {

class SystemPrivateGetIncognitoModeAvailabilityFunction
    : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("systemPrivate.getIncognitoModeAvailability",
                             SYSTEMPRIVATE_GETINCOGNITOMODEAVAILABILITY)

 protected:
  virtual ~SystemPrivateGetIncognitoModeAvailabilityFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class SystemPrivateGetUpdateStatusFunction
    : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("systemPrivate.getUpdateStatus",
                             SYSTEMPRIVATE_GETUPDATESTATUS)

 protected:
  virtual ~SystemPrivateGetUpdateStatusFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class SystemPrivateGetApiKeyFunction : public SyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("systemPrivate.getApiKey", SYSTEMPRIVATE_GETAPIKEY)

 protected:
  virtual ~SystemPrivateGetApiKeyFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

void DispatchBrightnessChangedEvent(int brightness, bool user_initiated);

void DispatchVolumeChangedEvent(double volume, bool is_volume_muted);

void DispatchScreenUnlockedEvent();

void DispatchWokeUpEvent();

}  

#endif  
