// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_NOTIFIER_SETTINGS_H_
#define UI_MESSAGE_CENTER_NOTIFIER_SETTINGS_H_

#include <string>

#include "base/gtest_prod_util.h"
#include "base/strings/string16.h"
#include "ui/gfx/image/image.h"
#include "ui/message_center/message_center_export.h"
#include "url/gurl.h"

FORWARD_DECLARE_TEST(MessageCenterTrayBridgeTest,
                     StatusItemOnlyAfterFirstNotification);

namespace ash {
class WebNotificationTrayTest;
}

namespace message_center {
namespace test {
class MessagePopupCollectionTest;
}

class NotifierSettingsDelegate;
class NotifierSettingsProvider;

MESSAGE_CENTER_EXPORT NotifierSettingsDelegate* ShowSettings(
    NotifierSettingsProvider* provider,
    gfx::NativeView context);

struct MESSAGE_CENTER_EXPORT NotifierId {
  enum NotifierType {
    APPLICATION,
    WEB_PAGE,
    SYSTEM_COMPONENT,
    SYNCED_NOTIFICATION_SERVICE,
  };

  
  NotifierId(NotifierType type, const std::string& id);

  
  explicit NotifierId(const GURL& url);

  bool operator==(const NotifierId& other) const;
  
  bool operator<(const NotifierId& other) const;

  NotifierType type;

  
  std::string id;

  
  GURL url;

  
  
  std::string profile_id;

 private:
  friend class MessageCenterTrayTest;
  friend class test::MessagePopupCollectionTest;
  friend class NotificationControllerTest;
  friend class PopupCollectionTest;
  friend class TrayViewControllerTest;
  friend class ash::WebNotificationTrayTest;
  FRIEND_TEST_ALL_PREFIXES(::MessageCenterTrayBridgeTest,
                           StatusItemOnlyAfterFirstNotification);
  FRIEND_TEST_ALL_PREFIXES(PopupControllerTest, Creation);
  FRIEND_TEST_ALL_PREFIXES(NotificationListTest, UnreadCountNoNegative);
  FRIEND_TEST_ALL_PREFIXES(NotificationListTest, TestHasNotificationOfType);

  
  NotifierId();
};

struct MESSAGE_CENTER_EXPORT Notifier {
  Notifier(const NotifierId& notifier_id, const string16& name, bool enabled);
  ~Notifier();

  NotifierId notifier_id;

  
  
  string16 name;

  
  bool enabled;

  
  gfx::Image icon;

 private:
  DISALLOW_COPY_AND_ASSIGN(Notifier);
};

struct MESSAGE_CENTER_EXPORT NotifierGroup {
  NotifierGroup(const gfx::Image& icon,
                const string16& name,
                const string16& login_info,
                size_t index);
  ~NotifierGroup();

  
  const gfx::Image icon;

  
  const string16 name;

  
  string16 login_info;

  
  
  const size_t index;

 private:
  DISALLOW_COPY_AND_ASSIGN(NotifierGroup);
};

class MESSAGE_CENTER_EXPORT NotifierSettingsObserver {
 public:
  
  virtual void UpdateIconImage(const NotifierId& notifier_id,
                               const gfx::Image& icon) = 0;

  
  virtual void NotifierGroupChanged() = 0;
};

class MESSAGE_CENTER_EXPORT NotifierSettingsProvider {
 public:
  virtual ~NotifierSettingsProvider() {};

  
  virtual void AddObserver(NotifierSettingsObserver* observer) = 0;
  virtual void RemoveObserver(NotifierSettingsObserver* observer) = 0;

  
  virtual size_t GetNotifierGroupCount() const = 0;

  
  virtual const message_center::NotifierGroup& GetNotifierGroupAt(
      size_t index) const = 0;

  
  virtual bool IsNotifierGroupActiveAt(size_t index) const = 0;

  
  
  virtual void SwitchToNotifierGroup(size_t index) = 0;

  
  virtual const message_center::NotifierGroup& GetActiveNotifierGroup()
      const = 0;

  
  
  virtual void GetNotifierList(std::vector<Notifier*>* notifiers) = 0;

  
  
  virtual void SetNotifierEnabled(const Notifier& notifier, bool enabled) = 0;

  
  virtual void OnNotifierSettingsClosing() = 0;

  
  
  virtual bool NotifierHasAdvancedSettings(const NotifierId& notifier_id)
      const = 0;

  
  virtual void OnNotifierAdvancedSettingsRequested(
      const NotifierId& notifier_id,
      const std::string* notification_id) = 0;
};

}  

#endif  
