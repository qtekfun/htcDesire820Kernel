// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef JINGLE_NOTIFIER_PUSH_NOTIFICATIONS_LISTENER_LISTEN_TASK_H_
#define JINGLE_NOTIFIER_PUSH_NOTIFICATIONS_LISTENER_LISTEN_TASK_H_

#include "base/compiler_specific.h"
#include "talk/xmpp/xmpptask.h"

namespace buzz {
class XmlElement;
}

namespace notifier {

struct Notification;

class PushNotificationsListenTask : public buzz::XmppTask {
 public:
  class Delegate {
   public:
    virtual void OnNotificationReceived(const Notification& notification) = 0;

   protected:
    virtual ~Delegate();
  };

  PushNotificationsListenTask(buzz::XmppTaskParentInterface* parent,
                              Delegate* delegate);
  virtual ~PushNotificationsListenTask();

  
  virtual int ProcessStart() OVERRIDE;
  virtual int ProcessResponse() OVERRIDE;
  virtual bool HandleStanza(const buzz::XmlElement* stanza) OVERRIDE;

 private:
  bool IsValidNotification(const buzz::XmlElement* stanza);

  Delegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(PushNotificationsListenTask);
};

typedef PushNotificationsListenTask::Delegate
    PushNotificationsListenTaskDelegate;

}  

#endif  
