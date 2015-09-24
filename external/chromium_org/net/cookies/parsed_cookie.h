// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_COOKIES_PARSED_COOKIE_H_
#define NET_COOKIES_PARSED_COOKIE_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "net/base/net_export.h"
#include "net/cookies/cookie_constants.h"

namespace net {

class NET_EXPORT ParsedCookie {
 public:
  typedef std::pair<std::string, std::string> TokenValuePair;
  typedef std::vector<TokenValuePair> PairList;

  
  static const size_t kMaxCookieSize = 4096;
  
  static const int kMaxPairs = 16;

  
  ParsedCookie(const std::string& cookie_line);
  ~ParsedCookie();

  
  
  bool IsValid() const;

  const std::string& Name() const { return pairs_[0].first; }
  const std::string& Token() const { return Name(); }
  const std::string& Value() const { return pairs_[0].second; }

  bool HasPath() const { return path_index_ != 0; }
  const std::string& Path() const { return pairs_[path_index_].second; }
  bool HasDomain() const { return domain_index_ != 0; }
  const std::string& Domain() const { return pairs_[domain_index_].second; }
  bool HasExpires() const { return expires_index_ != 0; }
  const std::string& Expires() const { return pairs_[expires_index_].second; }
  bool HasMaxAge() const { return maxage_index_ != 0; }
  const std::string& MaxAge() const { return pairs_[maxage_index_].second; }
  bool IsSecure() const { return secure_index_ != 0; }
  bool IsHttpOnly() const { return httponly_index_ != 0; }
  CookiePriority Priority() const;

  
  
  size_t NumberOfAttributes() const { return pairs_.size() - 1; }

  
  
  
  
  
  bool SetName(const std::string& name);
  bool SetValue(const std::string& value);
  bool SetPath(const std::string& path);
  bool SetDomain(const std::string& domain);
  bool SetExpires(const std::string& expires);
  bool SetMaxAge(const std::string& maxage);
  bool SetIsSecure(bool is_secure);
  bool SetIsHttpOnly(bool is_http_only);
  bool SetPriority(const std::string& priority);

  
  std::string ToCookieLine() const;

  
  
  static std::string::const_iterator FindFirstTerminator(const std::string& s);

  
  
  
  
  
  static bool ParseToken(std::string::const_iterator* it,
                         const std::string::const_iterator& end,
                         std::string::const_iterator* token_start,
                         std::string::const_iterator* token_end);

  
  
  
  
  static void ParseValue(std::string::const_iterator* it,
                         const std::string::const_iterator& end,
                         std::string::const_iterator* value_start,
                         std::string::const_iterator* value_end);

  
  
  static std::string ParseTokenString(const std::string& token);
  static std::string ParseValueString(const std::string& value);

 private:
  void ParseTokenValuePairs(const std::string& cookie_line);
  void SetupAttributes();

  
  
  
  
  
  
  bool SetString(size_t* index,
                 const std::string& key,
                 const std::string& value);
  bool SetBool(size_t* index,
               const std::string& key,
               bool value);

  
  
  bool SetAttributePair(size_t* index,
                        const std::string& key,
                        const std::string& value);

  
  
  void ClearAttributePair(size_t index);

  PairList pairs_;
  bool is_valid_;
  
  
  
  
  size_t path_index_;
  size_t domain_index_;
  size_t expires_index_;
  size_t maxage_index_;
  size_t secure_index_;
  size_t httponly_index_;
  size_t priority_index_;

  DISALLOW_COPY_AND_ASSIGN(ParsedCookie);
};

}  

#endif  
