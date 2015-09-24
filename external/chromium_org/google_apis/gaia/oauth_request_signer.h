// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_GAIA_OAUTH_REQUEST_SIGNER_H_
#define GOOGLE_APIS_GAIA_OAUTH_REQUEST_SIGNER_H_

#include <map>
#include <string>

#include "base/basictypes.h"

class GURL;

class OAuthRequestSigner {
 public:
  enum SignatureMethod {
    HMAC_SHA1_SIGNATURE,
    RSA_SHA1_SIGNATURE,
    PLAINTEXT_SIGNATURE
  };

  enum HttpMethod {
    GET_METHOD,
    POST_METHOD
  };

  typedef std::map<std::string,std::string> Parameters;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static bool Decode(const std::string& text, std::string* decoded_text);
  static std::string Encode(const std::string& text);

  
  
  
  
  static bool ParseAndSign(const GURL& request_url_with_parameters,
                           SignatureMethod signature_method,
                           HttpMethod http_method,
                           const std::string& consumer_key,
                           const std::string& consumer_secret,
                           const std::string& token_key,
                           const std::string& token_secret,
                           std::string* signed_result);

  
  
  
  
  
  
  
  static bool SignURL(const GURL& request_base_url,
                      const Parameters& parameters,
                      SignatureMethod signature_method,
                      HttpMethod http_method,
                      const std::string& consumer_key,
                      const std::string& consumer_secret,
                      const std::string& token_key,
                      const std::string& token_secret,
                      std::string* signed_result);

  
  
  static bool SignAuthHeader(const GURL& request_base_url,
                             const Parameters& parameters,
                             SignatureMethod signature_method,
                             HttpMethod http_method,
                             const std::string& consumer_key,
                             const std::string& consumer_secret,
                             const std::string& token_key,
                             const std::string& token_secret,
                             std::string* signed_result);

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(OAuthRequestSigner);
};

#endif  
