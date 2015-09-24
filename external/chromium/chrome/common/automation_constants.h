// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_AUTOMATION_CONSTANTS_H__
#define CHROME_COMMON_AUTOMATION_CONSTANTS_H__
#pragma once

namespace automation {

extern const char kJSONProxyAutoconfig[];
extern const char kJSONProxyNoProxy[];
extern const char kJSONProxyPacUrl[];
extern const char kJSONProxyBypassList[];
extern const char kJSONProxyServer[];

extern const char kNamedInterfacePrefix[];

static const int kSleepTime = 250;

enum KeyEventTypes {
  kRawKeyDownType = 0,
  kKeyDownType,
  kCharType,
  kKeyUpType,
};

enum KeyModifierMasks {
  kShiftKeyMask   = 1 << 0,
  kControlKeyMask = 1 << 1,
  kAltKeyMask     = 1 << 2,
  kMetaKeyMask    = 1 << 3,
};

enum MouseButton {
  kLeftButton = 0,
  kMiddleButton,
  kRightButton,
};

extern const int kChromeDriverAutomationVersion;

}  

enum AutomationLaunchResult {
  AUTOMATION_LAUNCH_RESULT_INVALID = -1,
  AUTOMATION_SUCCESS,
  AUTOMATION_TIMEOUT,
  AUTOMATION_VERSION_MISMATCH,
  AUTOMATION_CREATE_TAB_FAILED,
  AUTOMATION_SERVER_CRASHED,
};

enum AutomationMsg_NavigationResponseValues {
  AUTOMATION_MSG_NAVIGATION_ERROR = 0,
  AUTOMATION_MSG_NAVIGATION_SUCCESS,
  AUTOMATION_MSG_NAVIGATION_AUTH_NEEDED,
};

enum AutomationMsg_ExtensionResponseValues {
  AUTOMATION_MSG_EXTENSION_INSTALL_SUCCEEDED = 0,
  AUTOMATION_MSG_EXTENSION_INSTALL_FAILED
};

enum AutomationMsg_ExtensionProperty {
  AUTOMATION_MSG_EXTENSION_ID = 0,
  AUTOMATION_MSG_EXTENSION_NAME,
  AUTOMATION_MSG_EXTENSION_VERSION,
  AUTOMATION_MSG_EXTENSION_BROWSER_ACTION_INDEX,
};


#endif  
