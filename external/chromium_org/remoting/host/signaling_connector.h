// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_SIGNALING_CONNECTOR_H_
#define REMOTING_HOST_SIGNALING_CONNECTOR_H_

#include "base/basictypes.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "base/timer/timer.h"
#include "google_apis/gaia/gaia_oauth_client.h"
#include "net/base/network_change_notifier.h"
#include "remoting/jingle_glue/xmpp_signal_strategy.h"

namespace net {
class URLFetcher;
class URLRequestContextGetter;
}  

namespace remoting {

class DnsBlackholeChecker;

class SignalingConnector
    : public base::SupportsWeakPtr<SignalingConnector>,
      public base::NonThreadSafe,
      public SignalStrategy::Listener,
      public net::NetworkChangeNotifier::ConnectionTypeObserver,
      public net::NetworkChangeNotifier::IPAddressObserver,
      public gaia::GaiaOAuthClient::Delegate {
 public:
  
  
  struct OAuthCredentials {
    OAuthCredentials(const std::string& login_value,
                     const std::string& refresh_token_value,
                     bool is_service_account);

    
    std::string login;

    
    std::string refresh_token;

    
    bool is_service_account;
  };

  
  SignalingConnector(
      XmppSignalStrategy* signal_strategy,
      scoped_refptr<net::URLRequestContextGetter> url_request_context_getter,
      scoped_ptr<DnsBlackholeChecker> dns_blackhole_checker,
      const base::Closure& auth_failed_callback);
  virtual ~SignalingConnector();

  
  
  void EnableOAuth(scoped_ptr<OAuthCredentials> oauth_credentials);

  
  virtual void OnSignalStrategyStateChange(
      SignalStrategy::State state) OVERRIDE;
  virtual bool OnSignalStrategyIncomingStanza(
      const buzz::XmlElement* stanza) OVERRIDE;

  
  virtual void OnConnectionTypeChanged(
      net::NetworkChangeNotifier::ConnectionType type) OVERRIDE;

  
  virtual void OnIPAddressChanged() OVERRIDE;

  
  virtual void OnGetTokensResponse(const std::string& user_email,
                                   const std::string& access_token,
                                   int expires_seconds) OVERRIDE;
  virtual void OnRefreshTokenResponse(const std::string& access_token,
                                      int expires_in_seconds) OVERRIDE;
  virtual void OnGetUserEmailResponse(const std::string& user_email) OVERRIDE;
  virtual void OnOAuthError() OVERRIDE;
  virtual void OnNetworkError(int response_code) OVERRIDE;

 private:
  void ScheduleTryReconnect();
  void ResetAndTryReconnect();
  void TryReconnect();
  void OnDnsBlackholeCheckerDone(bool allow);

  void RefreshOAuthToken();

  XmppSignalStrategy* signal_strategy_;
  scoped_refptr<net::URLRequestContextGetter> url_request_context_getter_;
  base::Closure auth_failed_callback_;

  scoped_ptr<OAuthCredentials> oauth_credentials_;
  scoped_ptr<gaia::GaiaOAuthClient> gaia_oauth_client_;
  scoped_ptr<DnsBlackholeChecker> dns_blackhole_checker_;

  
  int reconnect_attempts_;

  bool refreshing_oauth_token_;
  std::string oauth_access_token_;
  base::Time auth_token_expiry_time_;

  base::OneShotTimer<SignalingConnector> timer_;

  DISALLOW_COPY_AND_ASSIGN(SignalingConnector);
};

}  

#endif  
