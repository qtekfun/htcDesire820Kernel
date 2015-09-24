// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_WEB_NOTIFICATION_WEB_NOTIFICATION_TRAY_H_
#define ASH_SYSTEM_WEB_NOTIFICATION_WEB_NOTIFICATION_TRAY_H_

#include "ash/ash_export.h"
#include "ash/system/tray/tray_background_view.h"
#include "ash/system/user/login_status.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "ui/base/models/simple_menu_model.h"
#include "ui/message_center/message_center_tray.h"
#include "ui/message_center/message_center_tray_delegate.h"
#include "ui/views/bubble/tray_bubble_view.h"
#include "ui/views/controls/button/button.h"


namespace views {
class ImageButton;
class MenuRunner;
}

namespace message_center {
class MessageBubbleBase;
class MessageCenter;
class MessageCenterBubble;
class MessagePopupCollection;
}

namespace ash {
namespace internal {
class StatusAreaWidget;
class WebNotificationBubbleWrapper;
class WebNotificationButton;
class WorkAreaObserver;
}

class ASH_EXPORT WebNotificationTray
    : public internal::TrayBackgroundView,
      public views::TrayBubbleView::Delegate,
      public message_center::MessageCenterTrayDelegate,
      public views::ButtonListener,
      public base::SupportsWeakPtr<WebNotificationTray>,
      public ui::SimpleMenuModel::Delegate {
 public:
  explicit WebNotificationTray(
      internal::StatusAreaWidget* status_area_widget);
  virtual ~WebNotificationTray();

  
  
  
  void SetSystemTrayHeight(int height);

  
  bool ShouldBlockLauncherAutoHide() const;

  
  bool IsMessageCenterBubbleVisible() const;

  
  bool IsMouseInNotificationBubble() const;

  
  void ShowMessageCenterBubble();

  
  void UpdateAfterLoginStatusChange(user::LoginStatus login_status);

  
  virtual void SetShelfAlignment(ShelfAlignment alignment) OVERRIDE;
  virtual void AnchorUpdated() OVERRIDE;
  virtual base::string16 GetAccessibleNameForTray() OVERRIDE;
  virtual void HideBubbleWithView(
      const views::TrayBubbleView* bubble_view) OVERRIDE;
  virtual bool ClickedOutsideBubble() OVERRIDE;

  
  virtual bool PerformAction(const ui::Event& event) OVERRIDE;

  
  virtual void BubbleViewDestroyed() OVERRIDE;
  virtual void OnMouseEnteredView() OVERRIDE;
  virtual void OnMouseExitedView() OVERRIDE;
  virtual base::string16 GetAccessibleNameForBubble() OVERRIDE;
  virtual gfx::Rect GetAnchorRect(views::Widget* anchor_widget,
                                  AnchorType anchor_type,
                                  AnchorAlignment anchor_alignment) OVERRIDE;
  virtual void HideBubble(const views::TrayBubbleView* bubble_view) OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual void OnMessageCenterTrayChanged() OVERRIDE;
  virtual bool ShowMessageCenter() OVERRIDE;
  virtual void HideMessageCenter() OVERRIDE;
  virtual bool ShowPopups() OVERRIDE;
  virtual void HidePopups() OVERRIDE;
  virtual bool ShowNotifierSettings() OVERRIDE;
  virtual message_center::MessageCenterTray* GetMessageCenterTray() OVERRIDE;

  
  virtual bool IsCommandIdChecked(int command_id) const OVERRIDE;
  virtual bool IsCommandIdEnabled(int command_id) const OVERRIDE;
  virtual bool GetAcceleratorForCommandId(
      int command_id,
      ui::Accelerator* accelerator) OVERRIDE;
  virtual void ExecuteCommand(int command_id, int event_flags) OVERRIDE;

  message_center::MessageCenter* message_center() const;

 private:
  friend class WebNotificationTrayTest;

  FRIEND_TEST_ALL_PREFIXES(WebNotificationTrayTest, WebNotifications);
  FRIEND_TEST_ALL_PREFIXES(WebNotificationTrayTest, WebNotificationPopupBubble);
  FRIEND_TEST_ALL_PREFIXES(WebNotificationTrayTest,
                           ManyMessageCenterNotifications);
  FRIEND_TEST_ALL_PREFIXES(WebNotificationTrayTest, ManyPopupNotifications);
  FRIEND_TEST_ALL_PREFIXES(WebNotificationTrayTest, PopupShownOnBothDisplays);
  FRIEND_TEST_ALL_PREFIXES(WebNotificationTrayTest, PopupAndSystemTray);
  FRIEND_TEST_ALL_PREFIXES(WebNotificationTrayTest, PopupAndAutoHideShelf);

  void UpdateTrayContent();

  
  
  
  bool ShowMessageCenterInternal(bool show_settings);

  
  
  bool ShouldShowMessageCenter();

  
  bool ShouldShowQuietModeMenu(const ui::Event& event);

  
  void ShowQuietModeMenu(const ui::Event& event);

  
  ui::MenuModel* CreateQuietModeMenu();

  internal::WebNotificationBubbleWrapper* message_center_bubble() const {
    return message_center_bubble_.get();
  }

  
  bool IsPopupVisible() const;
  message_center::MessageCenterBubble* GetMessageCenterBubbleForTest();

  scoped_ptr<message_center::MessageCenterTray> message_center_tray_;
  scoped_ptr<internal::WebNotificationBubbleWrapper> message_center_bubble_;
  scoped_ptr<message_center::MessagePopupCollection> popup_collection_;
  internal::WebNotificationButton* button_;

  bool show_message_center_on_unlock_;

  bool should_update_tray_content_;

  
  
  
  
  bool should_block_shelf_auto_hide_;

  
  scoped_ptr<internal::WorkAreaObserver> work_area_observer_;

  DISALLOW_COPY_AND_ASSIGN(WebNotificationTray);
};

}  

#endif  
