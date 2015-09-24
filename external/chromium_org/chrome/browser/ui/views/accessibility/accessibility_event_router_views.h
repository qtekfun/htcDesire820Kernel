// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_ACCESSIBILITY_ACCESSIBILITY_EVENT_ROUTER_VIEWS_H_
#define CHROME_BROWSER_UI_VIEWS_ACCESSIBILITY_ACCESSIBILITY_EVENT_ROUTER_VIEWS_H_

#include <string>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/strings/string16.h"
#include "chrome/browser/accessibility/accessibility_events.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/base/accessibility/accessibility_types.h"

class Profile;

template <typename T> struct DefaultSingletonTraits;

namespace views {
class View;
}

class AccessibilityEventRouterViews : public content::NotificationObserver {
 public:
  
  static AccessibilityEventRouterViews* GetInstance();

  
  void HandleAccessibilityEvent(
      views::View* view, ui::AccessibilityTypes::Event event_type);

  
  
  void HandleMenuItemFocused(const base::string16& menu_name,
                             const base::string16& menu_item_name,
                             int item_index,
                             int item_count,
                             bool has_submenu);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  friend struct DefaultSingletonTraits<AccessibilityEventRouterViews>;

  FRIEND_TEST_ALL_PREFIXES(AccessibilityEventRouterViewsTest,
                           TestFocusNotification);
  FRIEND_TEST_ALL_PREFIXES(AccessibilityEventRouterViewsTest,
                           MenuIndexAndCountForInvisibleMenu);

  AccessibilityEventRouterViews();
  virtual ~AccessibilityEventRouterViews();

  
  static void DispatchEventOnViewStorageId(
      int view_storage_id,
      ui::AccessibilityTypes::Event event);

  
  
  void DispatchAccessibilityEvent(
      views::View* view,
      ui::AccessibilityTypes::Event event);

  
  
  static void SendButtonNotification(
      views::View* view,
      ui::AccessibilityTypes::Event event,
      Profile* profile);
  static void SendLinkNotification(
      views::View* view,
      ui::AccessibilityTypes::Event event,
      Profile* profile);
  static void SendMenuNotification(
      views::View* view,
      ui::AccessibilityTypes::Event event,
      Profile* profile);
  static void SendMenuItemNotification(
      views::View* view,
      ui::AccessibilityTypes::Event event,
      Profile* profile);
  static void SendTreeNotification(
      views::View* view,
      ui::AccessibilityTypes::Event event,
      Profile* profile);
  static void SendTreeItemNotification(
      views::View* view,
      ui::AccessibilityTypes::Event event,
      Profile* profile);
  static void SendTextfieldNotification(
      views::View* view,
      ui::AccessibilityTypes::Event event,
      Profile* profile);
  static void SendComboboxNotification(
      views::View* view,
      ui::AccessibilityTypes::Event event,
      Profile* profile);
  static void SendCheckboxNotification(
      views::View* view,
      ui::AccessibilityTypes::Event event,
      Profile* profile);
  static void SendWindowNotification(
      views::View* view,
      ui::AccessibilityTypes::Event event,
      Profile* profile);
  static void SendSliderNotification(
      views::View* view,
      ui::AccessibilityTypes::Event event,
      Profile* profile);
  static void SendAlertControlNotification(
      views::View* view,
      ui::AccessibilityTypes::Event event,
      Profile* profile);

  
  static std::string GetViewName(views::View* view);

  
  static std::string GetViewContext(views::View* view);

  
  static views::View* FindDescendantWithAccessibleRole(
      views::View* view,
      ui::AccessibilityTypes::Role role);

  
  
  
  
  
  static void RecursiveGetMenuItemIndexAndCount(views::View* menu,
                                                views::View* item,
                                                int* index,
                                                int* count);

  
  
  static std::string RecursiveGetStaticText(views::View* view);

  
  
  
  Profile* most_recent_profile_;

  
  
  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(AccessibilityEventRouterViews);
};

#endif  
