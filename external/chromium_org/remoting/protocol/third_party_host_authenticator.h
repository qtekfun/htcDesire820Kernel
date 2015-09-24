// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_PROTOCOL_THIRD_PARTY_HOST_AUTHENTICATOR_H_
#define REMOTING_PROTOCOL_THIRD_PARTY_HOST_AUTHENTICATOR_H_

#include <string>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "remoting/protocol/third_party_authenticator_base.h"
#include "url/gurl.h"

namespace remoting {

class RsaKeyPair;

namespace protocol {

class ThirdPartyHostAuthenticator : public ThirdPartyAuthenticatorBase {
 public:
  class TokenValidator {
   public:
    
    
    
    
    typedef base::Callback<void(
                const std::string& shared_secret)> TokenValidatedCallback;

    virtual ~TokenValidator() {}

    
    
    
    
    virtual void ValidateThirdPartyToken(
        const std::string& token,
        const TokenValidatedCallback& token_validated_callback) = 0;

    
    virtual const GURL& token_url() const = 0;

    
    
    virtual const std::string& token_scope() const = 0;
  };

  class TokenValidatorFactory {
   public:
    virtual ~TokenValidatorFactory() {}

    
    
    virtual scoped_ptr<TokenValidator> CreateTokenValidator(
        const std::string& local_jid,
        const std::string& remote_jid) = 0;
  };

  
  
  
  
  ThirdPartyHostAuthenticator(const std::string& local_cert,
                              scoped_refptr<RsaKeyPair> key_pair,
                              scoped_ptr<TokenValidator> token_validator);
  virtual ~ThirdPartyHostAuthenticator();

 protected:
  
  virtual void ProcessTokenMessage(
      const buzz::XmlElement* message,
      const base::Closure& resume_callback) OVERRIDE;
  virtual void AddTokenElements(buzz::XmlElement* message) OVERRIDE;

 private:
  void OnThirdPartyTokenValidated(const buzz::XmlElement* message,
                                  const base::Closure& resume_callback,
                                  const std::string& shared_secret);

  std::string local_cert_;
  scoped_refptr<RsaKeyPair> key_pair_;
  scoped_ptr<TokenValidator> token_validator_;

  DISALLOW_COPY_AND_ASSIGN(ThirdPartyHostAuthenticator);
};

}  
}  

#endif  
