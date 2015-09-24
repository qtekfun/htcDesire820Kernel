// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_NOTIFICATION_BLOCKER_H_
#define UI_MESSAGE_CENTER_NOTIFICATION_BLOCKER_H_

#include "base/observer_list.h"
#include "ui/message_center/message_center_export.h"
#include "ui/message_center/notification.h"

namespace message_center {
class MessageCenter;

class MESSAGE_CENTER_EXPORT NotificationBlocker {
 public:
  class Observer {
   public:
    virtual void OnBlockingStateChanged(NotificationBlocker* blocker) = 0;
  };

  explicit NotificationBlocker(MessageCenter* message_center);
  virtual ~NotificationBlocker();

  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

  
  virtual void CheckState() {}

  
  
  virtual bool ShouldShowNotification(const NotifierId& notifier_id) const;

  
  
  
  
  virtual bool ShouldShowNotificationAsPopup(
      const NotifierId& notifier_id) const = 0;

 protected:
  MessageCenter* message_center() { return message_center_; }
  void NotifyBlockingStateChanged();

 private:
  ObserverList<Observer> observers_;
  MessageCenter* message_center_;  
};

typedef std::vector<NotificationBlocker*> NotificationBlockers;

}  

#endif  
