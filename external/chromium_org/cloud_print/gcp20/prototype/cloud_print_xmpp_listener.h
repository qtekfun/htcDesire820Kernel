// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CLOUD_PRINT_GCP20_PROTOTYPE_CLOUD_PRINT_XMPP_LISTENER_H_
#define CLOUD_PRINT_GCP20_PROTOTYPE_CLOUD_PRINT_XMPP_LISTENER_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "jingle/notifier/listener/push_client_observer.h"

namespace base {

class SingleThreadTaskRunner;

}  

namespace net {

class URLRequestContextGetter;

}  

namespace notifier {

class PushClient;

}  

class CloudPrintXmppListener
    : public base::SupportsWeakPtr<CloudPrintXmppListener>,
      public notifier::PushClientObserver {
 public:
  class Delegate {
   public:
    virtual ~Delegate() {}

    
    virtual void OnXmppConnected() = 0;

    
    virtual void OnXmppAuthError() = 0;

    
    virtual void OnXmppNetworkError() = 0;

    
    virtual void OnXmppNewPrintJob(const std::string& device_id) = 0;

    
    virtual void OnXmppNewLocalSettings(const std::string& device_id) = 0;

    
    virtual void OnXmppDeleteNotification(const std::string& device_id) = 0;
  };

  CloudPrintXmppListener(
      const std::string& robot_email,
      int standard_ping_interval,
      scoped_refptr<base::SingleThreadTaskRunner> task_runner,
      Delegate* delegate);

  virtual ~CloudPrintXmppListener();

  
  void Connect(const std::string& access_token);

  
  void set_ping_interval(int interval);

 private:
  
  virtual void OnNotificationsEnabled() OVERRIDE;
  virtual void OnNotificationsDisabled(
      notifier::NotificationsDisabledReason reason) OVERRIDE;
  virtual void OnIncomingNotification(
      const notifier::Notification& notification) OVERRIDE;
  virtual void OnPingResponse() OVERRIDE;

  
  void Disconnect();

  
  void SchedulePing();

  
  void SendPing();

  
  void OnPingTimeoutReached();

  
  std::string robot_email_;
  std::string access_token_;

  
  scoped_ptr<notifier::PushClient> push_client_;

  
  int standard_ping_interval_;

  
  
  int ping_timeouts_posted_;

  
  
  int ping_responses_pending_;

  
  bool ping_scheduled_;

  scoped_refptr<net::URLRequestContextGetter> context_getter_;

  Delegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(CloudPrintXmppListener);
};

#endif  

