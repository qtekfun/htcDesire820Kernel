// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_REGISTER_SUPPORT_HOST_REQUEST_H_
#define REMOTING_HOST_REGISTER_SUPPORT_HOST_REQUEST_H_

#include <string>

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "remoting/base/rsa_key_pair.h"
#include "remoting/jingle_glue/signal_strategy.h"
#include "testing/gtest/include/gtest/gtest_prod.h"

namespace buzz {
class XmlElement;
}  

namespace base {
class TimeDelta;
}  

namespace remoting {

class IqRequest;
class IqSender;

class RegisterSupportHostRequest : public SignalStrategy::Listener {
 public:
  
  
  
  typedef base::Callback<void(bool, const std::string&,
                              const base::TimeDelta&)> RegisterCallback;

  
  
  
  
  
  
  
  RegisterSupportHostRequest(SignalStrategy* signal_strategy,
                             scoped_refptr<RsaKeyPair> key_pair,
                             const std::string& directory_bot_jid,
                             const RegisterCallback& callback);
  virtual ~RegisterSupportHostRequest();

  
  virtual void OnSignalStrategyStateChange(
      SignalStrategy::State state) OVERRIDE;
  virtual bool OnSignalStrategyIncomingStanza(
      const buzz::XmlElement* stanza) OVERRIDE;

 private:
  void DoSend();

  scoped_ptr<buzz::XmlElement> CreateRegistrationRequest(
      const std::string& jid);
  scoped_ptr<buzz::XmlElement> CreateSignature(const std::string& jid);

  void ProcessResponse(IqRequest* request, const buzz::XmlElement* response);
  bool ParseResponse(const buzz::XmlElement* response,
                     std::string* support_id, base::TimeDelta* lifetime);

  void CallCallback(
      bool success, const std::string& support_id, base::TimeDelta lifetime);

  SignalStrategy* signal_strategy_;
  scoped_refptr<RsaKeyPair> key_pair_;
  std::string directory_bot_jid_;
  RegisterCallback callback_;

  scoped_ptr<IqSender> iq_sender_;
  scoped_ptr<IqRequest> request_;

  DISALLOW_COPY_AND_ASSIGN(RegisterSupportHostRequest);
};

}  

#endif  
