// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NOTIFICATIONS_MESSAGE_CENTER_STATS_COLLECTOR_H_
#define CHROME_BROWSER_NOTIFICATIONS_MESSAGE_CENTER_STATS_COLLECTOR_H_

#include <set>
#include <string>

#include "ui/message_center/message_center.h"
#include "ui/message_center/message_center_observer.h"
#include "ui/message_center/message_center_types.h"

namespace message_center {
class MessageCenter;
}

class MessageCenterStatsCollector
    : public message_center::MessageCenterObserver {
 public:
  enum NotificationActionType {
    NOTIFICATION_ACTION_UNKNOWN,
    NOTIFICATION_ACTION_ADD,
    NOTIFICATION_ACTION_UPDATE,
    NOTIFICATION_ACTION_CLICK,
    NOTIFICATION_ACTION_BUTTON_CLICK,
    NOTIFICATION_ACTION_DISPLAY,
    NOTIFICATION_ACTION_CLOSE_BY_USER,
    NOTIFICATION_ACTION_CLOSE_BY_SYSTEM,
    
    
    
    NOTIFICATION_ACTION_COUNT
  };

  explicit MessageCenterStatsCollector(
      message_center::MessageCenter* message_center);
  virtual ~MessageCenterStatsCollector();

 private:
  
  class NotificationStats {
   public:
    
    NotificationStats();

    explicit NotificationStats(const std::string& id);
    virtual ~NotificationStats();

    
    void CollectAction(NotificationActionType type);

    
    void RecordAggregateStats();

   private:
    std::string id_;
    bool actions_[NOTIFICATION_ACTION_COUNT];
  };

  
  virtual void OnNotificationAdded(const std::string& notification_id) OVERRIDE;
  virtual void OnNotificationRemoved(const std::string& notification_id,
                                     bool by_user) OVERRIDE;
  virtual void OnNotificationUpdated(
      const std::string& notification_id) OVERRIDE;
  virtual void OnNotificationClicked(
      const std::string& notification_id) OVERRIDE;
  virtual void OnNotificationButtonClicked(const std::string& notification_id,
                                           int button_index) OVERRIDE;
  virtual void OnNotificationDisplayed(
      const std::string& notification_id) OVERRIDE;
  virtual void OnCenterVisibilityChanged(
      message_center::Visibility visibility) OVERRIDE;
  virtual void OnQuietModeChanged(bool in_quiet_mode) OVERRIDE;

  
  message_center::MessageCenter* message_center_;

  typedef std::map<std::string,NotificationStats> StatsCollection;
  StatsCollection stats_;

  DISALLOW_COPY_AND_ASSIGN(MessageCenterStatsCollector);
};

#endif  
