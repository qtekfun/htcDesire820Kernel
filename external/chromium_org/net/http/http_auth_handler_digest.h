// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_AUTH_HANDLER_DIGEST_H_
#define NET_HTTP_HTTP_AUTH_HANDLER_DIGEST_H_

#include <string>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/net_export.h"
#include "net/http/http_auth_handler.h"
#include "net/http/http_auth_handler_factory.h"

namespace net {

class NET_EXPORT_PRIVATE HttpAuthHandlerDigest : public HttpAuthHandler {
 public:
  
  
  
  class NET_EXPORT_PRIVATE NonceGenerator {
   public:
    NonceGenerator();
    virtual ~NonceGenerator();

    
    virtual std::string GenerateNonce() const = 0;
   private:
    DISALLOW_COPY_AND_ASSIGN(NonceGenerator);
  };

  
  
  class DynamicNonceGenerator : public NonceGenerator {
   public:
    DynamicNonceGenerator();
    virtual std::string GenerateNonce() const OVERRIDE;
   private:
    DISALLOW_COPY_AND_ASSIGN(DynamicNonceGenerator);
  };

  
  
  class NET_EXPORT_PRIVATE FixedNonceGenerator : public NonceGenerator {
   public:
    explicit FixedNonceGenerator(const std::string& nonce);

    virtual std::string GenerateNonce() const OVERRIDE;

   private:
    const std::string nonce_;
    DISALLOW_COPY_AND_ASSIGN(FixedNonceGenerator);
  };

  class NET_EXPORT_PRIVATE Factory : public HttpAuthHandlerFactory {
   public:
    Factory();
    virtual ~Factory();

    
    void set_nonce_generator(const NonceGenerator* nonce_generator);

    virtual int CreateAuthHandler(
        HttpAuth::ChallengeTokenizer* challenge,
        HttpAuth::Target target,
        const GURL& origin,
        CreateReason reason,
        int digest_nonce_count,
        const BoundNetLog& net_log,
        scoped_ptr<HttpAuthHandler>* handler) OVERRIDE;

   private:
    scoped_ptr<const NonceGenerator> nonce_generator_;
  };

  virtual HttpAuth::AuthorizationResult HandleAnotherChallenge(
      HttpAuth::ChallengeTokenizer* challenge) OVERRIDE;

 protected:
  virtual bool Init(HttpAuth::ChallengeTokenizer* challenge) OVERRIDE;

  virtual int GenerateAuthTokenImpl(const AuthCredentials* credentials,
                                    const HttpRequestInfo* request,
                                    const CompletionCallback& callback,
                                    std::string* auth_token) OVERRIDE;

 private:
  FRIEND_TEST_ALL_PREFIXES(HttpAuthHandlerDigestTest, ParseChallenge);
  FRIEND_TEST_ALL_PREFIXES(HttpAuthHandlerDigestTest, AssembleCredentials);
  FRIEND_TEST_ALL_PREFIXES(HttpNetworkTransactionTest, DigestPreAuthNonceCount);

  
  enum DigestAlgorithm {
    
    
    ALGORITHM_UNSPECIFIED,

    
    ALGORITHM_MD5,

    
    
    ALGORITHM_MD5_SESS,
  };

  
  
  enum QualityOfProtection {
    QOP_UNSPECIFIED,
    QOP_AUTH,
  };

  
  
  
  
  
  HttpAuthHandlerDigest(int nonce_count, const NonceGenerator* nonce_generator);
  virtual ~HttpAuthHandlerDigest();

  
  
  bool ParseChallenge(HttpAuth::ChallengeTokenizer* challenge);

  
  bool ParseChallengeProperty(const std::string& name,
                              const std::string& value);

  
  static std::string GenerateNonce();

  
  static std::string QopToString(QualityOfProtection qop);
  static std::string AlgorithmToString(DigestAlgorithm algorithm);

  
  
  void GetRequestMethodAndPath(const HttpRequestInfo* request,
                               std::string* method,
                               std::string* path) const;

  
  std::string AssembleResponseDigest(const std::string& method,
                                     const std::string& path,
                                     const AuthCredentials& credentials,
                                     const std::string& cnonce,
                                     const std::string& nc) const;

  
  std::string AssembleCredentials(const std::string& method,
                                  const std::string& path,
                                  const AuthCredentials& credentials,
                                  const std::string& cnonce,
                                  int nonce_count) const;

  
  std::string nonce_;
  std::string domain_;
  std::string opaque_;
  bool stale_;
  DigestAlgorithm algorithm_;
  QualityOfProtection qop_;

  
  
  
  std::string original_realm_;

  int nonce_count_;
  const NonceGenerator* nonce_generator_;
};

}  

#endif  
