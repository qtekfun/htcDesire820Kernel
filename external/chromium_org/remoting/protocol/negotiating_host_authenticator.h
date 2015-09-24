// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_PROTOCOL_NEGOTIATING_HOST_AUTHENTICATOR_H_
#define REMOTING_PROTOCOL_NEGOTIATING_HOST_AUTHENTICATOR_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "remoting/protocol/authentication_method.h"
#include "remoting/protocol/authenticator.h"
#include "remoting/protocol/negotiating_authenticator_base.h"
#include "remoting/protocol/pairing_registry.h"
#include "remoting/protocol/third_party_host_authenticator.h"

namespace remoting {

class RsaKeyPair;

namespace protocol {

class NegotiatingHostAuthenticator : public NegotiatingAuthenticatorBase {
 public:
  virtual ~NegotiatingHostAuthenticator();

  
  
  
  static scoped_ptr<Authenticator> CreateWithSharedSecret(
      const std::string& local_cert,
      scoped_refptr<RsaKeyPair> key_pair,
      const std::string& shared_secret_hash,
      AuthenticationMethod::HashFunction hash_function,
      scoped_refptr<PairingRegistry> pairing_registry);

  
  static scoped_ptr<Authenticator> CreateWithThirdPartyAuth(
      const std::string& local_cert,
      scoped_refptr<RsaKeyPair> key_pair,
      scoped_ptr<ThirdPartyHostAuthenticator::TokenValidator> token_validator);

  
  virtual void ProcessMessage(const buzz::XmlElement* message,
                              const base::Closure& resume_callback) OVERRIDE;
  virtual scoped_ptr<buzz::XmlElement> GetNextMessage() OVERRIDE;

 private:
  NegotiatingHostAuthenticator(
      const std::string& local_cert,
      scoped_refptr<RsaKeyPair> key_pair);

  
  
  
  
  void CreateAuthenticator(Authenticator::State preferred_initial_state,
                           const base::Closure& resume_callback);

  std::string local_cert_;
  scoped_refptr<RsaKeyPair> local_key_pair_;

  
  std::string shared_secret_hash_;

  
  scoped_ptr<ThirdPartyHostAuthenticator::TokenValidator> token_validator_;

  
  scoped_refptr<PairingRegistry> pairing_registry_;

  DISALLOW_COPY_AND_ASSIGN(NegotiatingHostAuthenticator);
};

}  
}  

#endif  
