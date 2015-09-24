// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef JINGLE_NOTIFIER_LISTENER_NON_BLOCKING_PUSH_CLIENT_OBSERVER_H_
#define JINGLE_NOTIFIER_LISTENER_NON_BLOCKING_PUSH_CLIENT_OBSERVER_H_

#include "jingle/notifier/listener/notification_defines.h"

namespace notifier {

enum NotificationsDisabledReason {
  
  
  TRANSIENT_NOTIFICATION_ERROR,
  DEFAULT_NOTIFICATION_ERROR = TRANSIENT_NOTIFICATION_ERROR,
  
  NOTIFICATION_CREDENTIALS_REJECTED,
  
  
  NO_NOTIFICATION_ERROR
};

class PushClientObserver {
 protected:
  virtual ~PushClientObserver();

 public:
  
  virtual void OnNotificationsEnabled() = 0;

  
  
  virtual void OnNotificationsDisabled(
      NotificationsDisabledReason reason) = 0;

  
  
  virtual void OnIncomingNotification(const Notification& notification) = 0;

  
  
  virtual void OnPingResponse();
};

}  

#endif  
