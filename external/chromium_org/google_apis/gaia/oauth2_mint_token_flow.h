// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_GAIA_OAUTH2_MINT_TOKEN_FLOW_H_
#define GOOGLE_APIS_GAIA_OAUTH2_MINT_TOKEN_FLOW_H_

#include <string>
#include <vector>

#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "google_apis/gaia/oauth2_api_call_flow.h"

class GoogleServiceAuthError;
class OAuth2MintTokenFlowTest;

namespace base {
class DictionaryValue;
}

namespace content {
class URLFetcher;
}

namespace net {
class URLRequestContextGetter;
}

struct IssueAdviceInfoEntry {
 public:
  IssueAdviceInfoEntry();
  ~IssueAdviceInfoEntry();

  base::string16 description;
  std::vector<base::string16> details;

  bool operator==(const IssueAdviceInfoEntry& rhs) const;
};

typedef std::vector<IssueAdviceInfoEntry> IssueAdviceInfo;

class OAuth2MintTokenFlow : public OAuth2ApiCallFlow {
 public:
  
  
  enum Mode {
    
    MODE_ISSUE_ADVICE,
    
    MODE_RECORD_GRANT,
    
    MODE_MINT_TOKEN_NO_FORCE,
    
    MODE_MINT_TOKEN_FORCE,
  };

  
  struct Parameters {
   public:
    Parameters();
    Parameters(const std::string& at,
               const std::string& eid,
               const std::string& cid,
               const std::vector<std::string>& scopes_arg,
               Mode mode_arg);
    ~Parameters();

    std::string access_token;
    std::string extension_id;
    std::string client_id;
    std::vector<std::string> scopes;
    Mode mode;
  };

  class Delegate {
   public:
    virtual void OnMintTokenSuccess(const std::string& access_token,
                                    int time_to_live) {}
    virtual void OnIssueAdviceSuccess(const IssueAdviceInfo& issue_advice)  {}
    virtual void OnMintTokenFailure(const GoogleServiceAuthError& error) {}

   protected:
    virtual ~Delegate() {}
  };

  OAuth2MintTokenFlow(net::URLRequestContextGetter* context,
                      Delegate* delegate,
                      const Parameters& parameters);
  virtual ~OAuth2MintTokenFlow();

 protected:
  
  virtual GURL CreateApiCallUrl() OVERRIDE;
  virtual std::string CreateApiCallBody() OVERRIDE;

  virtual void ProcessApiCallSuccess(
      const net::URLFetcher* source) OVERRIDE;
  virtual void ProcessApiCallFailure(
      const net::URLFetcher* source) OVERRIDE;
  virtual void ProcessNewAccessToken(const std::string& access_token) OVERRIDE;
  virtual void ProcessMintAccessTokenFailure(
      const GoogleServiceAuthError& error) OVERRIDE;

 private:
  friend class OAuth2MintTokenFlowTest;
  FRIEND_TEST_ALL_PREFIXES(OAuth2MintTokenFlowTest, CreateApiCallBody);
  FRIEND_TEST_ALL_PREFIXES(OAuth2MintTokenFlowTest, ParseIssueAdviceResponse);
  FRIEND_TEST_ALL_PREFIXES(OAuth2MintTokenFlowTest, ParseMintTokenResponse);
  FRIEND_TEST_ALL_PREFIXES(OAuth2MintTokenFlowTest, ProcessApiCallSuccess);
  FRIEND_TEST_ALL_PREFIXES(OAuth2MintTokenFlowTest, ProcessApiCallFailure);
  FRIEND_TEST_ALL_PREFIXES(OAuth2MintTokenFlowTest,
      ProcessMintAccessTokenFailure);

  void ReportSuccess(const std::string& access_token, int time_to_live);
  void ReportIssueAdviceSuccess(const IssueAdviceInfo& issue_advice);
  void ReportFailure(const GoogleServiceAuthError& error);

  static bool ParseIssueAdviceResponse(
      const base::DictionaryValue* dict, IssueAdviceInfo* issue_advice);
  static bool ParseMintTokenResponse(
      const base::DictionaryValue* dict, std::string* access_token,
      int* time_to_live);

  Delegate* delegate_;
  Parameters parameters_;
  base::WeakPtrFactory<OAuth2MintTokenFlow> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(OAuth2MintTokenFlow);
};

#endif  
