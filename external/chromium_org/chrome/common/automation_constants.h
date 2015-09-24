// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_AUTOMATION_CONSTANTS_H__
#define CHROME_COMMON_AUTOMATION_CONSTANTS_H__

#include <string>

namespace automation {

extern const char kJSONProxyAutoconfig[];
extern const char kJSONProxyNoProxy[];
extern const char kJSONProxyPacUrl[];
extern const char kJSONProxyPacMandatory[];
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
  kNumLockKeyMask = 1 << 4,
};

enum MouseEventType {
  kMouseDown = 0,
  kMouseUp,
  kMouseMove,
  kMouseEnter,
  kMouseLeave,
  kContextMenu,
};

enum MouseButton {
  kLeftButton = 0,
  kMiddleButton,
  kRightButton,
  kNoButton,
};

}  

enum AutomationLaunchResult {
  AUTOMATION_LAUNCH_RESULT_INVALID = -1,
  AUTOMATION_SUCCESS,
  AUTOMATION_TIMEOUT,
  AUTOMATION_VERSION_MISMATCH,
  AUTOMATION_CREATE_TAB_FAILED,
  AUTOMATION_SERVER_CRASHED,
  AUTOMATION_CHANNEL_ERROR,
};

enum AutomationMsg_NavigationResponseValues {
  AUTOMATION_MSG_NAVIGATION_ERROR = 0,
  AUTOMATION_MSG_NAVIGATION_SUCCESS,
  AUTOMATION_MSG_NAVIGATION_AUTH_NEEDED,
  AUTOMATION_MSG_NAVIGATION_BLOCKED_BY_MODAL_DIALOG,
};

enum AutomationMsg_DEPRECATED_ExtensionProperty {
  AUTOMATION_MSG_EXTENSION_ID = 0,
  AUTOMATION_MSG_EXTENSION_NAME,
  AUTOMATION_MSG_EXTENSION_VERSION,
  AUTOMATION_MSG_EXTENSION_BROWSER_ACTION_INDEX,
};

enum AutomationPageFontSize {
  SMALLEST_FONT = 8,
  SMALL_FONT = 12,
  MEDIUM_FONT = 16,
  LARGE_FONT = 24,
  LARGEST_FONT = 36
};

enum FindInPageDirection { BACK = 0, FWD = 1 };
enum FindInPageCase { IGNORE_CASE = 0, CASE_SENSITIVE = 1 };

#endif  
