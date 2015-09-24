// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_PROTOCOL_NEGOTIATING_CLIENT_AUTHENTICATOR_H_
#define REMOTING_PROTOCOL_NEGOTIATING_CLIENT_AUTHENTICATOR_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "remoting/protocol/authentication_method.h"
#include "remoting/protocol/authenticator.h"
#include "remoting/protocol/negotiating_authenticator_base.h"
#include "remoting/protocol/third_party_client_authenticator.h"

namespace remoting {
namespace protocol {

class NegotiatingClientAuthenticator : public NegotiatingAuthenticatorBase {
 public:
  
  NegotiatingClientAuthenticator(
      const std::string& client_pairing_id,
      const std::string& shared_secret,
      const std::string& authentication_tag,
      const FetchSecretCallback& fetch_secret_callback,
      scoped_ptr<ThirdPartyClientAuthenticator::TokenFetcher> token_fetcher_,
      const std::vector<AuthenticationMethod>& methods);

  virtual ~NegotiatingClientAuthenticator();

  
  virtual void ProcessMessage(const buzz::XmlElement* message,
                              const base::Closure& resume_callback) OVERRIDE;
  virtual scoped_ptr<buzz::XmlElement> GetNextMessage() OVERRIDE;

 private:
  
  
  
  
  void CreateAuthenticatorForCurrentMethod(
      Authenticator::State preferred_initial_state,
      const base::Closure& resume_callback);

  
  
  
  
  
  
  
  
  
  void CreatePreferredAuthenticator();

  
  
  void CreateV2AuthenticatorWithSecret(
      Authenticator::State initial_state,
      const base::Closure& resume_callback,
      const std::string& shared_secret);

  
  std::string client_pairing_id_;
  std::string shared_secret_;

  
  std::string authentication_tag_;

  
  FetchSecretCallback fetch_secret_callback_;

  
  scoped_ptr<ThirdPartyClientAuthenticator::TokenFetcher> token_fetcher_;

  
  bool method_set_by_host_;
  base::WeakPtrFactory<NegotiatingClientAuthenticator> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(NegotiatingClientAuthenticator);
};

}  
}  

#endif  
