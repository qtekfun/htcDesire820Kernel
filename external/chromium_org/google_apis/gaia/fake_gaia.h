// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_GAIA_FAKE_GAIA_H_
#define GOOGLE_APIS_GAIA_FAKE_GAIA_H_

#include <map>
#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"

namespace base {
class DictionaryValue;
}

namespace net {
namespace test_server {
class BasicHttpResponse;
struct HttpRequest;
class HttpResponse;
}
}

class FakeGaia {
 public:
  typedef std::set<std::string> ScopeSet;

  
  struct AccessTokenInfo {
    AccessTokenInfo();
    ~AccessTokenInfo();

    std::string token;
    std::string issued_to;
    std::string audience;
    std::string user_id;
    ScopeSet scopes;
    int expires_in;
    std::string email;
  };

  FakeGaia();
  ~FakeGaia();

  
  
  
  scoped_ptr<net::test_server::HttpResponse> HandleRequest(
      const net::test_server::HttpRequest& request);

  
  
  
  
  void IssueOAuthToken(const std::string& auth_token,
                       const AccessTokenInfo& token_info);

 private:
  typedef std::multimap<std::string, AccessTokenInfo> AccessTokenInfoMap;

  
  void FormatJSONResponse(const base::DictionaryValue& response_dict,
                          net::test_server::BasicHttpResponse* http_response);

  
  
  
  
  const AccessTokenInfo* GetAccessTokenInfo(const std::string& auth_token,
                                            const std::string& client_id,
                                            const std::string& scope_string)
      const;

  
  
  static bool GetQueryParameter(const std::string& query,
                                const std::string& key,
                                std::string* value);

  AccessTokenInfoMap access_token_info_map_;
  std::string service_login_response_;

  DISALLOW_COPY_AND_ASSIGN(FakeGaia);
};

#endif  
