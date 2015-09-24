// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_STATUS_SENDER_H_
#define REMOTING_HOST_STATUS_SENDER_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "remoting/base/rsa_key_pair.h"
#include "remoting/host/host_exit_codes.h"
#include "remoting/jingle_glue/signal_strategy.h"

namespace base {
class MessageLoopProxy;
}  

namespace buzz {
class XmlElement;
}  

namespace remoting {

class RsaKeyPair;
class IqSender;

class HostStatusSender : SignalStrategy::Listener {
 public:

  enum HostStatus {
    OFFLINE = 0,
    ONLINE = 1
  };

  HostStatusSender(const std::string& host_id,
                   SignalStrategy* signal_strategy,
                   scoped_refptr<RsaKeyPair> key_pair,
                   const std::string& directory_bot_jid);
  virtual ~HostStatusSender();

  
  virtual void OnSignalStrategyStateChange(
      SignalStrategy::State state) OVERRIDE;
  virtual bool OnSignalStrategyIncomingStanza(
      const buzz::XmlElement* stanza) OVERRIDE;

  
  
  void SendOnlineStatus();
  void SendOfflineStatus(HostExitCodes exit_code);

  inline static const char* HostStatusToString(HostStatus host_status) {
    return host_status_strings_[host_status];
  }

 private:
  
  void SendHostStatus(HostStatus status, HostExitCodes exit_code);

  
  scoped_ptr<buzz::XmlElement> CreateHostStatusMessage(
      HostStatus status, HostExitCodes exit_code);

  
  scoped_ptr<buzz::XmlElement> CreateSignature(
      HostStatus status, HostExitCodes exit_code);

  std::string host_id_;
  SignalStrategy* signal_strategy_;
  scoped_refptr<RsaKeyPair> key_pair_;
  std::string directory_bot_jid_;
  scoped_ptr<IqSender> iq_sender_;

  
  static const char* const host_status_strings_[2];

  DISALLOW_COPY_AND_ASSIGN(HostStatusSender);
};

}  

#endif  
