// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_HEARTBEAT_SENDER_H_
#define REMOTING_HOST_HEARTBEAT_SENDER_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/timer/timer.h"
#include "remoting/base/rsa_key_pair.h"
#include "remoting/jingle_glue/signal_strategy.h"

namespace base {
class MessageLoopProxy;
}  

namespace buzz {
class XmlElement;
}  

namespace remoting {

class RsaKeyPair;
class IqRequest;
class IqSender;

class HeartbeatSender : public SignalStrategy::Listener {
 public:
  class Listener {
   public:
    virtual ~Listener() { }

    
    virtual void OnHeartbeatSuccessful() = 0;

    
    virtual void OnUnknownHostIdError() = 0;
  };

  
  
  
  HeartbeatSender(Listener* listener,
                  const std::string& host_id,
                  SignalStrategy* signal_strategy,
                  scoped_refptr<RsaKeyPair> key_pair,
                  const std::string& directory_bot_jid);
  virtual ~HeartbeatSender();

  
  virtual void OnSignalStrategyStateChange(
      SignalStrategy::State state) OVERRIDE;
  virtual bool OnSignalStrategyIncomingStanza(
      const buzz::XmlElement* stanza) OVERRIDE;

 private:
  FRIEND_TEST_ALL_PREFIXES(HeartbeatSenderTest, DoSendStanza);
  FRIEND_TEST_ALL_PREFIXES(HeartbeatSenderTest,
                           DoSendStanzaWithExpectedSequenceId);
  FRIEND_TEST_ALL_PREFIXES(HeartbeatSenderTest, CreateHeartbeatMessage);
  FRIEND_TEST_ALL_PREFIXES(HeartbeatSenderTest, ProcessResponseSetInterval);
  FRIEND_TEST_ALL_PREFIXES(HeartbeatSenderTest,
                           ProcessResponseExpectedSequenceId);

  void SendStanza();
  void ResendStanza();
  void DoSendStanza();
  void ProcessResponse(IqRequest* request, const buzz::XmlElement* response);
  void SetInterval(int interval);
  void SetSequenceId(int sequence_id);

  
  scoped_ptr<buzz::XmlElement> CreateHeartbeatMessage();
  scoped_ptr<buzz::XmlElement> CreateSignature();

  Listener* listener_;
  std::string host_id_;
  SignalStrategy* signal_strategy_;
  scoped_refptr<RsaKeyPair> key_pair_;
  std::string directory_bot_jid_;
  scoped_ptr<IqSender> iq_sender_;
  scoped_ptr<IqRequest> request_;
  int interval_ms_;
  base::RepeatingTimer<HeartbeatSender> timer_;
  base::OneShotTimer<HeartbeatSender> timer_resend_;
  int sequence_id_;
  bool sequence_id_was_set_;
  int sequence_id_recent_set_num_;
  bool heartbeat_succeeded_;
  int failed_startup_heartbeat_count_;

  DISALLOW_COPY_AND_ASSIGN(HeartbeatSender);
};

}  

#endif  
