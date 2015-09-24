// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_JINGLE_GLUE_IQ_SENDER_H_
#define REMOTING_JINGLE_GLUE_IQ_SENDER_H_

#include <map>
#include <string>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "remoting/jingle_glue/signal_strategy.h"

namespace base {
class TimeDelta;
}  

namespace buzz {
class XmlElement;
}  

namespace remoting {

class IqRequest;
class SignalStrategy;

class IqSender : public SignalStrategy::Listener {
 public:
  
  
  typedef base::Callback<void(IqRequest* request,
                              const buzz::XmlElement* response)> ReplyCallback;

  explicit IqSender(SignalStrategy* signal_strategy);
  virtual ~IqSender();

  
  
  
  
  
  scoped_ptr<IqRequest> SendIq(scoped_ptr<buzz::XmlElement> stanza,
                               const ReplyCallback& callback);

  
  scoped_ptr<IqRequest> SendIq(const std::string& type,
                               const std::string& addressee,
                               scoped_ptr<buzz::XmlElement> iq_body,
                               const ReplyCallback& callback);

  
  virtual void OnSignalStrategyStateChange(
      SignalStrategy::State state) OVERRIDE;
  virtual bool OnSignalStrategyIncomingStanza(
      const buzz::XmlElement* stanza) OVERRIDE;

 private:
  typedef std::map<std::string, IqRequest*> IqRequestMap;
  friend class IqRequest;

  
  static scoped_ptr<buzz::XmlElement> MakeIqStanza(
      const std::string& type,
      const std::string& addressee,
      scoped_ptr<buzz::XmlElement> iq_body);

  
  void RemoveRequest(IqRequest* request);

  SignalStrategy* signal_strategy_;
  IqRequestMap requests_;

  DISALLOW_COPY_AND_ASSIGN(IqSender);
};

class IqRequest : public  base::SupportsWeakPtr<IqRequest> {
 public:
  IqRequest(IqSender* sender, const IqSender::ReplyCallback& callback,
            const std::string& addressee);
  ~IqRequest();

  
  
  void SetTimeout(base::TimeDelta timeout);

 private:
  friend class IqSender;

  void CallCallback(const buzz::XmlElement* stanza);
  void OnTimeout();

  
  void OnResponse(const buzz::XmlElement* stanza);

  void DeliverResponse(scoped_ptr<buzz::XmlElement> stanza);

  IqSender* sender_;
  IqSender::ReplyCallback callback_;
  std::string addressee_;

  DISALLOW_COPY_AND_ASSIGN(IqRequest);
};

}  

#endif  
