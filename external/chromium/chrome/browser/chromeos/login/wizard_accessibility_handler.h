// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_WIZARD_ACCESSIBILITY_HANDLER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_WIZARD_ACCESSIBILITY_HANDLER_H_
#pragma once

#include <string>

#include "base/gtest_prod_util.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_source.h"
#include "content/common/notification_type.h"

class AccessibilityControlInfo;
class AccessibilityTextBoxInfo;

namespace chromeos {

enum EarconType {
  NO_EARCON,
  EARCON_BUTTON,
  EARCON_CHECK_OFF,
  EARCON_CHECK_ON,
  EARCON_ELLIPSES,
  EARCON_LINK,
  EARCON_LISTBOX,
  EARCON_MENU,
  EARCON_OBJECT_OPENED,
  EARCON_OBJECT_CLOSED,
  EARCON_TAB,
  EARCON_TEXTBOX,
};

class WizardAccessibilityHandler : public NotificationObserver {
 public:
  WizardAccessibilityHandler() { }

  
  void Speak(const char* speak_str, bool queue, bool interruptible);

 private:
  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  void DescribeAccessibilityEvent(NotificationType event_type,
                                  const AccessibilityControlInfo* control_info,
                                  std::string* out_spoken_description,
                                  EarconType* out_earcon);

  
  
  void DescribeControl(const AccessibilityControlInfo* control_info,
                       bool is_action,
                       std::string* out_spoken_description,
                       EarconType* out_earcon);

  
  
  void DescribeTextChanged(const AccessibilityControlInfo* control_info,
                           std::string* out_spoken_description,
                           EarconType* out_earcon);

  
  
  std::string GetTextBoxValue(const AccessibilityTextBoxInfo* textbox_info);

  
  void DescribeTextSelectionChanged(const std::string& value,
                                    int old_start, int old_end,
                                    int new_start, int new_end,
                                    std::string* out_spoken_description);

  
  void DescribeTextContentsChanged(const std::string& old_value,
                                   const std::string& new_value,
                                   std::string* out_spoken_description);

  int previous_text_selection_start_;
  int previous_text_selection_end_;
  std::string previous_text_value_;

  friend class WizardAccessibilityHandlerTest;
  FRIEND_TEST_ALL_PREFIXES(WizardAccessibilityHandlerTest, TestFocusEvents);
  FRIEND_TEST_ALL_PREFIXES(WizardAccessibilityHandlerTest, TestTextEvents);

  DISALLOW_COPY_AND_ASSIGN(WizardAccessibilityHandler);
};

}  

#endif  
