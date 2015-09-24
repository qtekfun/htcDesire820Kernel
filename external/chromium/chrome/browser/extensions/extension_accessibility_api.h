// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_ACCESSIBILITY_API_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_ACCESSIBILITY_API_H_
#pragma once

#include <string>
#include <vector>

#include "base/callback.h"
#include "base/memory/singleton.h"
#include "base/values.h"
#include "chrome/browser/accessibility_events.h"
#include "chrome/browser/extensions/extension_function.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class ExtensionAccessibilityEventRouter : public NotificationObserver {
 public:
  
  static ExtensionAccessibilityEventRouter* GetInstance();

  
  void ObserveProfile(Profile* profile);

  
  
  DictionaryValue* last_focused_control_dict() {
    return &last_focused_control_dict_;
  }

  
  
  
  
  
  void SetAccessibilityEnabled(bool enabled);
  bool IsAccessibilityEnabled() const;
  typedef Callback0::Type Callback;
  void AddOnEnabledListener(Callback* callback);
  void AddOnDisabledListener(Callback* callback);

 private:
  friend struct DefaultSingletonTraits<ExtensionAccessibilityEventRouter>;

  ExtensionAccessibilityEventRouter();
  virtual ~ExtensionAccessibilityEventRouter();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  void OnWindowOpened(const AccessibilityWindowInfo* details);
  void OnWindowClosed(const AccessibilityWindowInfo* details);
  void OnControlFocused(const AccessibilityControlInfo* details);
  void OnControlAction(const AccessibilityControlInfo* details);
  void OnTextChanged(const AccessibilityControlInfo* details);
  void OnMenuOpened(const AccessibilityMenuInfo* details);
  void OnMenuClosed(const AccessibilityMenuInfo* details);

  void DispatchEvent(Profile* profile,
                     const char* event_name,
                     const std::string& json_args);

  
  NotificationRegistrar registrar_;

  DictionaryValue last_focused_control_dict_;

  bool enabled_;
  std::vector<Callback*> on_enabled_listeners_;
  std::vector<Callback*> on_disabled_listeners_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionAccessibilityEventRouter);
};

class SetAccessibilityEnabledFunction : public SyncExtensionFunction {
  virtual ~SetAccessibilityEnabledFunction() {}
  virtual bool RunImpl();
  DECLARE_EXTENSION_FUNCTION_NAME(
      "experimental.accessibility.setAccessibilityEnabled")
};

class GetFocusedControlFunction : public SyncExtensionFunction {
  virtual ~GetFocusedControlFunction() {}
  virtual bool RunImpl();
  DECLARE_EXTENSION_FUNCTION_NAME(
      "experimental.accessibility.getFocusedControl")
};

#endif  
