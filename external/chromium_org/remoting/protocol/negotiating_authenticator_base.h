// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_PROTOCOL_NEGOTIATING_AUTHENTICATOR_BASE_H_
#define REMOTING_PROTOCOL_NEGOTIATING_AUTHENTICATOR_BASE_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "remoting/protocol/authentication_method.h"
#include "remoting/protocol/authenticator.h"
#include "third_party/libjingle/source/talk/xmllite/xmlelement.h"

namespace remoting {
namespace protocol {

class NegotiatingAuthenticatorBase : public Authenticator {
 public:
  virtual ~NegotiatingAuthenticatorBase();

  
  virtual State state() const OVERRIDE;
  virtual RejectionReason rejection_reason() const OVERRIDE;
  virtual scoped_ptr<ChannelAuthenticator>
      CreateChannelAuthenticator() const OVERRIDE;

  
  
  void ProcessMessageInternal(const buzz::XmlElement* message,
                              const base::Closure& resume_callback);

  const AuthenticationMethod& current_method_for_testing() const {
    return current_method_;
  }

 protected:
  static const buzz::StaticQName kMethodAttributeQName;
  static const buzz::StaticQName kSupportedMethodsAttributeQName;
  static const char kSupportedMethodsSeparator;

  explicit NegotiatingAuthenticatorBase(Authenticator::State initial_state);

  void AddMethod(const AuthenticationMethod& method);

  
  
  void UpdateState(const base::Closure& resume_callback);

  
  
  virtual scoped_ptr<buzz::XmlElement> GetNextMessageInternal();

  std::vector<AuthenticationMethod> methods_;
  AuthenticationMethod current_method_;
  scoped_ptr<Authenticator> current_authenticator_;
  State state_;
  RejectionReason rejection_reason_;

 private:
  DISALLOW_COPY_AND_ASSIGN(NegotiatingAuthenticatorBase);
};

}  
}  

#endif  
