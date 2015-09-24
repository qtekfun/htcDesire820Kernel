// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef JINGLE_NOTIFIER_LISTENER_PUSH_CLIENT_H_
#define JINGLE_NOTIFIER_LISTENER_PUSH_CLIENT_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "jingle/notifier/listener/notification_defines.h"

namespace notifier {

struct NotifierOptions;
class PushClientObserver;

class PushClient {
 public:
  virtual ~PushClient();

  
  
  static scoped_ptr<PushClient> CreateDefault(
      const NotifierOptions& notifier_options);

  
  
  
  static scoped_ptr<PushClient> CreateDefaultOnIOThread(
      const NotifierOptions& notifier_options);

  
  virtual void AddObserver(PushClientObserver* observer) = 0;
  virtual void RemoveObserver(PushClientObserver* observer) = 0;

  
  
  
  virtual void UpdateSubscriptions(const SubscriptionList& subscriptions) = 0;

  
  
  
  virtual void UpdateCredentials(
      const std::string& email, const std::string& token) = 0;

  
  virtual void SendNotification(const Notification& notification) = 0;

  
  virtual void SendPing() = 0;
};

}  

#endif  
