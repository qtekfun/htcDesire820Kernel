// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_MESSAGE_CENTER_OBSERVER_H_
#define UI_MESSAGE_CENTER_MESSAGE_CENTER_OBSERVER_H_

#include <string>

#include "ui/message_center/message_center_export.h"
#include "ui/message_center/message_center_types.h"

namespace message_center {
class NotificationBlocker;

class MESSAGE_CENTER_EXPORT MessageCenterObserver {
 public:
  virtual ~MessageCenterObserver() {}

  
  
  virtual void OnNotificationAdded(const std::string& notification_id) {}

  
  
  virtual void OnNotificationRemoved(const std::string& notification_id,
                                     bool by_user) {}

  
  
  virtual void OnNotificationUpdated(const std::string& notification_id) {}

  
  
  virtual void OnNotificationClicked(const std::string& notification_id) {}

  
  
  virtual void OnNotificationButtonClicked(const std::string& notification_id,
                                           int button_index) {}

  
  
  virtual void OnNotificationDisplayed(const std::string& notification_id) {}

  
  virtual void OnCenterVisibilityChanged(Visibility visibility) {}

  
  
  virtual void OnQuietModeChanged(bool in_quiet_mode) {}

  
  virtual void OnBlockingStateChanged(NotificationBlocker* blocker) {}
};

}  

#endif  
