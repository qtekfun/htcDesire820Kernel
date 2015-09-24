// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_MESSAGE_CENTER_H_
#define UI_MESSAGE_CENTER_MESSAGE_CENTER_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/message_center/message_center_export.h"
#include "ui/message_center/message_center_types.h"
#include "ui/message_center/notification_list.h"
#include "ui/message_center/notification_types.h"

namespace base {
class DictionaryValue;
}


namespace message_center {

namespace test {
class MessagePopupCollectionTest;
}

class MessageCenterObserver;
class NotificationBlocker;
class NotificationList;
class NotifierSettingsDelegate;
class NotifierSettingsProvider;

class MESSAGE_CENTER_EXPORT MessageCenter {
 public:
  
  static void Initialize();

  
  
  static MessageCenter* Get();

  
  static void Shutdown();

  
  virtual void AddObserver(MessageCenterObserver* observer) = 0;
  virtual void RemoveObserver(MessageCenterObserver* observer) = 0;

  
  virtual size_t NotificationCount() const = 0;
  virtual size_t UnreadNotificationCount() const = 0;
  virtual bool HasPopupNotifications() const = 0;
  virtual bool HasNotification(const std::string& id) = 0;
  virtual bool IsQuietMode() const = 0;
  virtual bool HasClickedListener(const std::string& id) = 0;

  
  
  
  virtual const NotificationList::Notifications& GetVisibleNotifications() = 0;

  
  
  
  virtual NotificationList::PopupNotifications GetPopupNotifications() = 0;

  
  virtual void AddNotificationBlocker(NotificationBlocker* blocker) = 0;
  virtual void RemoveNotificationBlocker(NotificationBlocker* blocker) = 0;

  

  
  virtual void AddNotification(scoped_ptr<Notification> notification) = 0;

  
  virtual void UpdateNotification(
      const std::string& old_id,
      scoped_ptr<Notification> new_notification) = 0;

  
  virtual void RemoveNotification(const std::string& id, bool by_user) = 0;
  virtual void RemoveAllNotifications(bool by_user) = 0;
  virtual void RemoveAllVisibleNotifications(bool by_user) = 0;

  
  virtual void SetNotificationIcon(const std::string& notification_id,
                                   const gfx::Image& image) = 0;

  
  
  virtual void SetNotificationImage(const std::string& notification_id,
                                    const gfx::Image& image) = 0;

  
  virtual void SetNotificationButtonIcon(const std::string& notification_id,
                                         int button_index,
                                         const gfx::Image& image) = 0;

  
  
  
  
  virtual void DisableNotificationsByNotifier(
      const NotifierId& notifier_id) = 0;

  
  virtual void ExpandNotification(const std::string& id) = 0;

  
  
  
  virtual void ClickOnNotification(const std::string& id) = 0;

  
  
  
  virtual void ClickOnNotificationButton(const std::string& id,
                                         int button_index) = 0;

  
  
  
  
  virtual void MarkSinglePopupAsShown(const std::string& id,
                                      bool mark_notification_as_read) = 0;

  
  
  
  virtual void DisplayedNotification(const std::string& id) = 0;

  
  
  
  virtual void SetNotifierSettingsProvider(
      NotifierSettingsProvider* provider) = 0;
  virtual NotifierSettingsProvider* GetNotifierSettingsProvider() = 0;

  
  virtual void SetQuietMode(bool in_quiet_mode) = 0;

  
  virtual void EnterQuietModeWithExpire(const base::TimeDelta& expires_in) = 0;

  
  
  virtual void SetVisibility(Visibility visible) = 0;

  
  virtual bool IsMessageCenterVisible() const = 0;

  
  
  virtual void PausePopupTimers() = 0;

  
  
  virtual void RestartPopupTimers() = 0;

 protected:
  friend class TrayViewControllerTest;
  friend class test::MessagePopupCollectionTest;
  virtual void DisableTimersForTest() = 0;

  MessageCenter();
  virtual ~MessageCenter();

 private:
  DISALLOW_COPY_AND_ASSIGN(MessageCenter);
};

}  

#endif  
