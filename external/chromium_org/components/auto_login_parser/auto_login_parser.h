// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTO_LOGIN_PARSER_AUTO_LOGIN_PARSER_H_
#define COMPONENTS_AUTO_LOGIN_PARSER_AUTO_LOGIN_PARSER_H_

#include <string>

namespace net {
class URLRequest;
}

namespace auto_login_parser {

enum RealmRestriction {
  ONLY_GOOGLE_COM,
  ALLOW_ANY_REALM
};

struct HeaderData {
  HeaderData();
  ~HeaderData();

  
  std::string realm;

  
  std::string account;

  
  
  std::string args;
};

bool ParseHeader(const std::string& header,
                 RealmRestriction realm_restriction,
                 HeaderData* header_data);

bool ParserHeaderInResponse(net::URLRequest* request,
                            RealmRestriction realm_restriction,
                            HeaderData* header_data);

}  

#endif  
