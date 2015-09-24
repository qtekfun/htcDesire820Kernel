// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef JINGLE_NOTIFIER_LISTENER_PUSH_NOTIFICATIONS_SEND_UPDATE_TASK_H_
#define JINGLE_NOTIFIER_LISTENER_PUSH_NOTIFICATIONS_SEND_UPDATE_TASK_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "jingle/notifier/listener/notification_defines.h"
#include "talk/xmpp/xmpptask.h"

namespace buzz {
class Jid;
class XmlElement;
}  

namespace notifier {

class PushNotificationsSendUpdateTask : public buzz::XmppTask {
 public:
  PushNotificationsSendUpdateTask(
      buzz::XmppTaskParentInterface* parent, const Notification& notification);
  virtual ~PushNotificationsSendUpdateTask();

  
  virtual int ProcessStart() OVERRIDE;

 private:
  
  static buzz::XmlElement* MakeUpdateMessage(
      const Notification& notification, const buzz::Jid& to_jid_bare);

  const Notification notification_;

  FRIEND_TEST_ALL_PREFIXES(PushNotificationsSendUpdateTaskTest,
                           MakeUpdateMessage);

  DISALLOW_COPY_AND_ASSIGN(PushNotificationsSendUpdateTask);
};

}  

#endif  
