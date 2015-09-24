// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_PROTOCOL_THIRD_PARTY_CLIENT_AUTHENTICATOR_H_
#define REMOTING_PROTOCOL_THIRD_PARTY_CLIENT_AUTHENTICATOR_H_

#include <string>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "remoting/protocol/third_party_authenticator_base.h"
#include "url/gurl.h"

class GURL;

namespace remoting {
namespace protocol {

class ThirdPartyClientAuthenticator : public ThirdPartyAuthenticatorBase {
 public:
  class TokenFetcher {
   public:
    
    
    
    
    
    typedef base::Callback<void(
        const std::string& token,
        const std::string& shared_secret)> TokenFetchedCallback;

    virtual ~TokenFetcher() {}

    
    
    
    
    
    
    
    virtual void FetchThirdPartyToken(
        const GURL& token_url,
        const std::string& scope,
        const TokenFetchedCallback& token_fetched_callback) = 0;
  };

  
  
  explicit ThirdPartyClientAuthenticator(
      scoped_ptr<TokenFetcher> token_fetcher);
  virtual ~ThirdPartyClientAuthenticator();

 protected:
  
  virtual void ProcessTokenMessage(
      const buzz::XmlElement* message,
      const base::Closure& resume_callback) OVERRIDE;
  virtual void AddTokenElements(buzz::XmlElement* message) OVERRIDE;

 private:
  void OnThirdPartyTokenFetched(const base::Closure& resume_callback,
                                const std::string& third_party_token,
                                const std::string& shared_secret);

  std::string token_;
  scoped_ptr<TokenFetcher> token_fetcher_;

  DISALLOW_COPY_AND_ASSIGN(ThirdPartyClientAuthenticator);
};


}  
}  

#endif  
