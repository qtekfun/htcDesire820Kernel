// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_PROTOCOL_THIRD_PARTY_AUTHENTICATOR_BASE_H_
#define REMOTING_PROTOCOL_THIRD_PARTY_AUTHENTICATOR_BASE_H_

#include <string>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "remoting/protocol/authenticator.h"
#include "third_party/libjingle/source/talk/xmllite/qname.h"

namespace buzz {

class XmlElement;

}  

namespace remoting {
namespace protocol {

class ThirdPartyAuthenticatorBase : public Authenticator {
 public:
  virtual ~ThirdPartyAuthenticatorBase();

  
  virtual State state() const OVERRIDE;
  virtual RejectionReason rejection_reason() const OVERRIDE;
  virtual void ProcessMessage(const buzz::XmlElement* message,
                              const base::Closure& resume_callback) OVERRIDE;
  virtual scoped_ptr<buzz::XmlElement> GetNextMessage() OVERRIDE;
  virtual scoped_ptr<ChannelAuthenticator>
      CreateChannelAuthenticator() const OVERRIDE;

 protected:
  
  static const buzz::StaticQName kTokenUrlTag;
  static const buzz::StaticQName kTokenScopeTag;
  static const buzz::StaticQName kTokenTag;

  explicit ThirdPartyAuthenticatorBase(State initial_state);

  
  void ProcessUnderlyingMessage(
      const buzz::XmlElement* message,
      const base::Closure& resume_callback);

  
  virtual void ProcessTokenMessage(
      const buzz::XmlElement* message,
      const base::Closure& resume_callback) = 0;

  
  virtual void AddTokenElements(buzz::XmlElement* message) = 0;

  scoped_ptr<Authenticator> underlying_;
  State token_state_;
  RejectionReason rejection_reason_;

 private:
  DISALLOW_COPY_AND_ASSIGN(ThirdPartyAuthenticatorBase);
};

}  
}  

#endif  
