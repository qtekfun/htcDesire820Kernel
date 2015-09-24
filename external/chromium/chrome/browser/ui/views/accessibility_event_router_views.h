// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_ACCESSIBILITY_EVENT_ROUTER_VIEWS_H_
#define CHROME_BROWSER_UI_VIEWS_ACCESSIBILITY_EVENT_ROUTER_VIEWS_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/singleton.h"
#include "base/task.h"
#include "chrome/browser/accessibility_events.h"
#include "ui/base/accessibility/accessibility_types.h"

class Profile;
namespace views {
class View;
}

class AccessibilityEventRouterViews {
 public:
  
  
  struct ViewInfo {
    ViewInfo() : ignore(false) {}

    
    std::string name;

    
    bool ignore;
  };

  
  static AccessibilityEventRouterViews* GetInstance();

  
  void HandleAccessibilityEvent(
      views::View* view, ui::AccessibilityTypes::Event event_type);

  
  
  void HandleMenuItemFocused(
      const std::wstring& menu_name,
      const std::wstring& menu_item_name,
      int item_index,
      int item_count,
      bool has_submenu);

 private:
  AccessibilityEventRouterViews();
  virtual ~AccessibilityEventRouterViews();

  friend struct DefaultSingletonTraits<AccessibilityEventRouterViews>;
  FRIEND_TEST_ALL_PREFIXES(AccessibilityEventRouterViewsTest,
                           TestFocusNotification);

  
  
  void DispatchAccessibilityNotification(
      views::View* view, NotificationType type);

  
  std::string GetViewName(views::View* view);

  
  
  void SendButtonNotification(
      views::View* view, NotificationType type, Profile* profile);
  void SendLinkNotification(
      views::View* view, NotificationType type, Profile* profile);
  void SendMenuNotification(
      views::View* view, NotificationType type, Profile* profile);
  void SendMenuItemNotification(
      views::View* view, NotificationType type, Profile* profile);
  void SendLocationBarNotification(
      views::View* view, NotificationType type, Profile* profile);
  void SendTextfieldNotification(
      views::View* view, NotificationType type, Profile* profile);
  void SendComboboxNotification(
      views::View* view, NotificationType type, Profile* profile);
  void SendCheckboxNotification(
      views::View* view, NotificationType type, Profile* profile);

  
  bool IsMenuEvent(views::View* view, NotificationType type);

  
  
  
  
  
  void RecursiveGetMenuItemIndexAndCount(
      views::View* menu, views::View* item, int* index, int* count);

  
  
  
  Profile* most_recent_profile_;

  
  
  ScopedRunnableMethodFactory<AccessibilityEventRouterViews> method_factory_;
};

#endif  
