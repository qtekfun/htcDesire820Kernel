// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_ACCESSIBILITY_ACCESSIBILITY_EXTENSION_API_H_
#define CHROME_BROWSER_ACCESSIBILITY_ACCESSIBILITY_EXTENSION_API_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/memory/singleton.h"
#include "base/values.h"
#include "chrome/browser/accessibility/accessibility_events.h"
#include "chrome/browser/extensions/chrome_extension_function.h"
#include "ui/base/accessibility/accessibility_types.h"

namespace extensions {
class ExtensionHost;
} 

class ExtensionAccessibilityEventRouter {
 public:
  typedef base::Callback<void(ui::AccessibilityTypes::Event,
                              const AccessibilityControlInfo*)>
      ControlEventCallback;
  
  static ExtensionAccessibilityEventRouter* GetInstance();

  
  
  DictionaryValue* last_focused_control_dict() {
    return &last_focused_control_dict_;
  }

  
  
  
  
  void SetAccessibilityEnabled(bool enabled);
  bool IsAccessibilityEnabled() const;

  
  
  void SetControlEventCallbackForTesting(ControlEventCallback callback);
  void ClearControlEventCallback();

  
  void HandleWindowEvent(ui::AccessibilityTypes::Event event,
                         const AccessibilityWindowInfo* info);

  
  void HandleMenuEvent(ui::AccessibilityTypes::Event event,
                       const AccessibilityMenuInfo* info);

  
  void HandleControlEvent(ui::AccessibilityTypes::Event event,
                          const AccessibilityControlInfo* info);

  void OnChromeVoxLoadStateChanged(
      Profile* profile,
      bool loading,
      bool make_announcements);

  static void DispatchEventToChromeVox(
      Profile* profile,
      const char* event_name,
      scoped_ptr<base::ListValue> event_args);

 private:
  friend struct DefaultSingletonTraits<ExtensionAccessibilityEventRouter>;

  ExtensionAccessibilityEventRouter();
  virtual ~ExtensionAccessibilityEventRouter();

  void OnWindowOpened(const AccessibilityWindowInfo* details);
  void OnControlFocused(const AccessibilityControlInfo* details);
  void OnControlAction(const AccessibilityControlInfo* details);
  void OnTextChanged(const AccessibilityControlInfo* details);
  void OnMenuOpened(const AccessibilityMenuInfo* details);
  void OnMenuClosed(const AccessibilityMenuInfo* details);

  void DispatchEvent(Profile* profile,
                     const char* event_name,
                     scoped_ptr<base::ListValue> event_args);

  DictionaryValue last_focused_control_dict_;

  bool enabled_;

  
  ControlEventCallback control_event_callback_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionAccessibilityEventRouter);
};

class AccessibilitySetAccessibilityEnabledFunction
    : public ChromeSyncExtensionFunction {
  virtual ~AccessibilitySetAccessibilityEnabledFunction() {}
  virtual bool RunImpl() OVERRIDE;
  DECLARE_EXTENSION_FUNCTION(
      "experimental.accessibility.setAccessibilityEnabled",
      EXPERIMENTAL_ACCESSIBILITY_SETACCESSIBILITYENABLED)
};

class AccessibilitySetNativeAccessibilityEnabledFunction
    : public ChromeSyncExtensionFunction {
  virtual ~AccessibilitySetNativeAccessibilityEnabledFunction() {}
  virtual bool RunImpl() OVERRIDE;
  DECLARE_EXTENSION_FUNCTION(
      "experimental.accessibility.setNativeAccessibilityEnabled",
      EXPERIMENTAL_ACCESSIBILITY_SETNATIVEACCESSIBILITYENABLED)
};

class AccessibilityGetFocusedControlFunction
    : public ChromeSyncExtensionFunction {
  virtual ~AccessibilityGetFocusedControlFunction() {}
  virtual bool RunImpl() OVERRIDE;
  DECLARE_EXTENSION_FUNCTION(
      "experimental.accessibility.getFocusedControl",
      EXPERIMENTAL_ACCESSIBILITY_GETFOCUSEDCONTROL)
};

class AccessibilityGetAlertsForTabFunction
    : public ChromeSyncExtensionFunction {
  virtual ~AccessibilityGetAlertsForTabFunction() {}
  virtual bool RunImpl() OVERRIDE;
  DECLARE_EXTENSION_FUNCTION(
      "experimental.accessibility.getAlertsForTab",
      EXPERIMENTAL_ACCESSIBILITY_GETALERTSFORTAB)
};

#endif  
