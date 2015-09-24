// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef JINGLE_NOTIFIER_COMMUNICATOR_LOGIN_H_
#define JINGLE_NOTIFIER_COMMUNICATOR_LOGIN_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "jingle/notifier/base/server_information.h"
#include "jingle/notifier/communicator/login_settings.h"
#include "jingle/notifier/communicator/single_login_attempt.h"
#include "net/base/network_change_notifier.h"
#include "talk/xmpp/xmppengine.h"

namespace buzz {
class XmppClient;
class XmppClientSettings;
class XmppTaskParentInterface;
}  

namespace net {
class URLRequestContextGetter;
}  

namespace notifier {

class LoginSettings;

class Login : public net::NetworkChangeNotifier::IPAddressObserver,
              public net::NetworkChangeNotifier::ConnectionTypeObserver,
              public net::NetworkChangeNotifier::DNSObserver,
              public SingleLoginAttempt::Delegate {
 public:
  class Delegate {
   public:
    
    virtual void OnConnect(
        base::WeakPtr<buzz::XmppTaskParentInterface> base_task) = 0;

    
    
    
    virtual void OnTransientDisconnection() = 0;

    
    
    
    
    virtual void OnCredentialsRejected() = 0;

   protected:
    virtual ~Delegate();
  };

  
  Login(Delegate* delegate,
        const buzz::XmppClientSettings& user_settings,
        const scoped_refptr<net::URLRequestContextGetter>&
            request_context_getter,
        const ServerList& servers,
        bool try_ssltcp_first,
        const std::string& auth_mechanism);
  virtual ~Login();

  
  void StartConnection();

  
  
  
  void UpdateXmppSettings(const buzz::XmppClientSettings& user_settings);

  
  virtual void OnIPAddressChanged() OVERRIDE;

  
  virtual void OnConnectionTypeChanged(
      net::NetworkChangeNotifier::ConnectionType type) OVERRIDE;

  
  virtual void OnDNSChanged() OVERRIDE;

  
  virtual void OnConnect(
      base::WeakPtr<buzz::XmppTaskParentInterface> base_task) OVERRIDE;
  virtual void OnRedirect(const ServerInformation& redirect_server) OVERRIDE;
  virtual void OnCredentialsRejected() OVERRIDE;
  virtual void OnSettingsExhausted() OVERRIDE;

 private:
  
  void OnNetworkEvent();

  
  
  void ResetReconnectState();

  
  
  void TryReconnect();

  
  
  void DoReconnect();

  Delegate* const delegate_;
  LoginSettings login_settings_;
  scoped_ptr<SingleLoginAttempt> single_attempt_;

  
  base::TimeDelta reconnect_interval_;
  base::OneShotTimer<Login> reconnect_timer_;

  DISALLOW_COPY_AND_ASSIGN(Login);
};

}  

#endif  
