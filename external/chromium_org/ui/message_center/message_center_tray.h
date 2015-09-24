// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_MESSAGE_CENTER_TRAY_H_
#define UI_MESSAGE_CENTER_MESSAGE_CENTER_TRAY_H_

#include "base/observer_list.h"
#include "ui/message_center/message_center_export.h"
#include "ui/message_center/message_center_observer.h"
#include "ui/message_center/message_center_tray_delegate.h"

namespace message_center {

class MessageCenter;
class MessageBubbleBase;
class MessagePopupBubble;
class QuietModeBubble;

MessageCenterTrayDelegate* CreateMessageCenterTray();

class MESSAGE_CENTER_EXPORT MessageCenterTray : public MessageCenterObserver {
 public:
  MessageCenterTray(MessageCenterTrayDelegate* delegate,
                    message_center::MessageCenter* message_center);
  virtual ~MessageCenterTray();

  
  
  
  bool ShowMessageCenterBubble();

  
  
  bool HideMessageCenterBubble();

  
  
  void MarkMessageCenterHidden();

  void ToggleMessageCenterBubble();

  
  void ShowPopupBubble();

  
  bool HidePopupBubble();

  
  void ShowNotifierSettingsBubble();

  bool message_center_visible() { return message_center_visible_; }
  bool popups_visible() { return popups_visible_; }
  MessageCenterTrayDelegate* delegate() { return delegate_; }
  const message_center::MessageCenter* message_center() const {
    return message_center_;
  }
  message_center::MessageCenter* message_center() { return message_center_; }

  
  virtual void OnNotificationAdded(const std::string& notification_id) OVERRIDE;
  virtual void OnNotificationRemoved(const std::string& notification_id,
                                     bool by_user) OVERRIDE;
  virtual void OnNotificationUpdated(
      const std::string& notification_id) OVERRIDE;
  virtual void OnNotificationClicked(
      const std::string& notification_id) OVERRIDE;
  virtual void OnNotificationButtonClicked(
      const std::string& notification_id,
      int button_index) OVERRIDE;
  virtual void OnNotificationDisplayed(
      const std::string& notification_id) OVERRIDE;
  virtual void OnQuietModeChanged(bool in_quiet_mode) OVERRIDE;
  virtual void OnBlockingStateChanged(NotificationBlocker* blocker) OVERRIDE;

 private:
  void OnMessageCenterChanged();
  void NotifyMessageCenterTrayChanged();
  void HidePopupBubbleInternal();

  
  
  message_center::MessageCenter* message_center_;
  bool message_center_visible_;
  bool popups_visible_;
  
  MessageCenterTrayDelegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(MessageCenterTray);
};

}  

#endif  
