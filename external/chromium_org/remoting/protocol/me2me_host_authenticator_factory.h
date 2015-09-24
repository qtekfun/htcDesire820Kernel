// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_PROTOCOL_ME2ME_HOST_AUTHENTICATOR_FACTORY_H_
#define REMOTING_PROTOCOL_ME2ME_HOST_AUTHENTICATOR_FACTORY_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "remoting/protocol/authentication_method.h"
#include "remoting/protocol/authenticator.h"
#include "remoting/protocol/third_party_host_authenticator.h"

namespace remoting {

class RsaKeyPair;

namespace protocol {

class PairingRegistry;

class Me2MeHostAuthenticatorFactory : public AuthenticatorFactory {
 public:
  
  static scoped_ptr<AuthenticatorFactory> CreateWithSharedSecret(
      bool use_service_account,
      const std::string& host_owner,
      const std::string& local_cert,
      scoped_refptr<RsaKeyPair> key_pair,
      const SharedSecretHash& shared_secret_hash,
      scoped_refptr<PairingRegistry> pairing_registry);

  
  static scoped_ptr<AuthenticatorFactory> CreateWithThirdPartyAuth(
      bool use_service_account,
      const std::string& host_owner,
      const std::string& local_cert,
      scoped_refptr<RsaKeyPair> key_pair,
      scoped_ptr<ThirdPartyHostAuthenticator::TokenValidatorFactory>
          token_validator_factory);

  
  
  static scoped_ptr<AuthenticatorFactory> CreateRejecting();

  Me2MeHostAuthenticatorFactory();
  virtual ~Me2MeHostAuthenticatorFactory();

  
  virtual scoped_ptr<Authenticator> CreateAuthenticator(
      const std::string& local_jid,
      const std::string& remote_jid,
      const buzz::XmlElement* first_message) OVERRIDE;

 private:
  
  bool use_service_account_;
  std::string host_owner_;
  std::string local_cert_;
  scoped_refptr<RsaKeyPair> key_pair_;

  
  SharedSecretHash shared_secret_hash_;

  
  scoped_ptr<ThirdPartyHostAuthenticator::TokenValidatorFactory>
      token_validator_factory_;

  
  scoped_refptr<PairingRegistry> pairing_registry_;

  DISALLOW_COPY_AND_ASSIGN(Me2MeHostAuthenticatorFactory);
};

}  
}  

#endif  
