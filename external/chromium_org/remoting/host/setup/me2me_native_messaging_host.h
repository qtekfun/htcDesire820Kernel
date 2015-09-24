// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_SETUP_ME2ME_NATIVE_MESSAGING_HOST_H_
#define REMOTING_HOST_SETUP_ME2ME_NATIVE_MESSAGING_HOST_H_

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/thread_checker.h"
#include "remoting/host/native_messaging/native_messaging_channel.h"
#include "remoting/host/setup/daemon_controller.h"
#include "remoting/host/setup/oauth_client.h"

namespace base {
class DictionaryValue;
class ListValue;
}  

namespace gaia {
class GaiaOAuthClient;
}  

namespace remoting {

namespace protocol {
class PairingRegistry;
}  

class Me2MeNativeMessagingHost {
 public:
  typedef NativeMessagingChannel::SendMessageCallback SendMessageCallback;

  Me2MeNativeMessagingHost(
      scoped_ptr<NativeMessagingChannel> channel,
      scoped_refptr<DaemonController> daemon_controller,
      scoped_refptr<protocol::PairingRegistry> pairing_registry,
      scoped_ptr<OAuthClient> oauth_client);
  virtual ~Me2MeNativeMessagingHost();

  void Start(const base::Closure& quit_closure);

 private:
  void ProcessMessage(scoped_ptr<base::DictionaryValue> message);

  
  
  
  bool ProcessHello(
      const base::DictionaryValue& message,
      scoped_ptr<base::DictionaryValue> response);
  bool ProcessClearPairedClients(
      const base::DictionaryValue& message,
      scoped_ptr<base::DictionaryValue> response);
  bool ProcessDeletePairedClient(
      const base::DictionaryValue& message,
      scoped_ptr<base::DictionaryValue> response);
  bool ProcessGetHostName(
      const base::DictionaryValue& message,
      scoped_ptr<base::DictionaryValue> response);
  bool ProcessGetPinHash(
      const base::DictionaryValue& message,
      scoped_ptr<base::DictionaryValue> response);
  bool ProcessGenerateKeyPair(
      const base::DictionaryValue& message,
      scoped_ptr<base::DictionaryValue> response);
  bool ProcessUpdateDaemonConfig(
      const base::DictionaryValue& message,
      scoped_ptr<base::DictionaryValue> response);
  bool ProcessGetDaemonConfig(
      const base::DictionaryValue& message,
      scoped_ptr<base::DictionaryValue> response);
  bool ProcessGetPairedClients(
      const base::DictionaryValue& message,
      scoped_ptr<base::DictionaryValue> response);
  bool ProcessGetUsageStatsConsent(
      const base::DictionaryValue& message,
      scoped_ptr<base::DictionaryValue> response);
  bool ProcessStartDaemon(
      const base::DictionaryValue& message,
      scoped_ptr<base::DictionaryValue> response);
  bool ProcessStopDaemon(
      const base::DictionaryValue& message,
      scoped_ptr<base::DictionaryValue> response);
  bool ProcessGetDaemonState(
      const base::DictionaryValue& message,
      scoped_ptr<base::DictionaryValue> response);
  bool ProcessGetHostClientId(
      const base::DictionaryValue& message,
      scoped_ptr<base::DictionaryValue> response);
  bool ProcessGetCredentialsFromAuthCode(
      const base::DictionaryValue& message,
      scoped_ptr<base::DictionaryValue> response);

  
  
  
  
  void SendConfigResponse(scoped_ptr<base::DictionaryValue> response,
                          scoped_ptr<base::DictionaryValue> config);
  void SendPairedClientsResponse(scoped_ptr<base::DictionaryValue> response,
                                 scoped_ptr<base::ListValue> pairings);
  void SendUsageStatsConsentResponse(
      scoped_ptr<base::DictionaryValue> response,
      const DaemonController::UsageStatsConsent& consent);
  void SendAsyncResult(scoped_ptr<base::DictionaryValue> response,
                       DaemonController::AsyncResult result);
  void SendBooleanResult(scoped_ptr<base::DictionaryValue> response,
                         bool result);
  void SendCredentialsResponse(scoped_ptr<base::DictionaryValue> response,
                               const std::string& user_email,
                               const std::string& refresh_token);

  scoped_ptr<NativeMessagingChannel> channel_;
  scoped_refptr<DaemonController> daemon_controller_;

  
  scoped_refptr<protocol::PairingRegistry> pairing_registry_;

  
  scoped_ptr<OAuthClient> oauth_client_;

  base::ThreadChecker thread_checker_;

  base::WeakPtr<Me2MeNativeMessagingHost> weak_ptr_;
  base::WeakPtrFactory<Me2MeNativeMessagingHost> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(Me2MeNativeMessagingHost);
};

int Me2MeNativeMessagingHostMain();

}  

#endif  
