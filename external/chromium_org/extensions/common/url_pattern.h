// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef EXTENSIONS_COMMON_URL_PATTERN_H_
#define EXTENSIONS_COMMON_URL_PATTERN_H_

#include <functional>
#include <string>
#include <vector>

class GURL;

class URLPattern {
 public:
  
  enum SchemeMasks {
    SCHEME_NONE       = 0,
    SCHEME_HTTP       = 1 << 0,
    SCHEME_HTTPS      = 1 << 1,
    SCHEME_FILE       = 1 << 2,
    SCHEME_FTP        = 1 << 3,
    SCHEME_CHROMEUI   = 1 << 4,
    SCHEME_EXTENSION  = 1 << 5,
    SCHEME_FILESYSTEM = 1 << 6,

    
    
    
    
    
    
    SCHEME_ALL      = -1,
  };

  
  enum ParseResult {
    PARSE_SUCCESS = 0,
    PARSE_ERROR_MISSING_SCHEME_SEPARATOR,
    PARSE_ERROR_INVALID_SCHEME,
    PARSE_ERROR_WRONG_SCHEME_SEPARATOR,
    PARSE_ERROR_EMPTY_HOST,
    PARSE_ERROR_INVALID_HOST_WILDCARD,
    PARSE_ERROR_EMPTY_PATH,
    PARSE_ERROR_INVALID_PORT,
    NUM_PARSE_RESULTS
  };

  
  static const char kAllUrlsPattern[];

  explicit URLPattern(int valid_schemes);

  
  
  URLPattern(int valid_schemes, const std::string& pattern);

  URLPattern();
  ~URLPattern();

  bool operator<(const URLPattern& other) const;
  bool operator>(const URLPattern& other) const;
  bool operator==(const URLPattern& other) const;

  
  
  
  
  ParseResult Parse(const std::string& pattern_str);

  
  int valid_schemes() const { return valid_schemes_; }
  void SetValidSchemes(int valid_schemes);

  
  
  const std::string& host() const { return host_; }
  void SetHost(const std::string& host);

  
  bool match_subdomains() const { return match_subdomains_; }
  void SetMatchSubdomains(bool val);

  
  
  const std::string& path() const { return path_; }
  void SetPath(const std::string& path);

  
  bool match_all_urls() const { return match_all_urls_; }
  void SetMatchAllURLs(bool val);

  
  
  
  bool SetScheme(const std::string& scheme);
  
  
  const std::string& scheme() const { return scheme_; }

  
  
  bool IsValidScheme(const std::string& scheme) const;

  
  bool MatchesURL(const GURL& test) const;

  
  bool MatchesSecurityOrigin(const GURL& test) const;

  
  
  
  
  bool MatchesScheme(const std::string& test) const;

  
  bool MatchesHost(const std::string& test) const;
  bool MatchesHost(const GURL& test) const;

  
  bool MatchesPath(const std::string& test) const;

  
  bool SetPort(const std::string& port);
  const std::string& port() const { return port_; }

  
  const std::string& GetAsString() const;

  
  
  
  bool OverlapsWith(const URLPattern& other) const;

  
  
  bool Contains(const URLPattern& other) const;

  
  
  
  
  std::vector<URLPattern> ConvertToExplicitSchemes() const;

  static bool EffectiveHostCompare(const URLPattern& a, const URLPattern& b) {
    if (a.match_all_urls_ && b.match_all_urls_)
      return false;
    return a.host_.compare(b.host_) < 0;
  };

  
  class EffectiveHostCompareFunctor {
   public:
    bool operator()(const URLPattern& a, const URLPattern& b) const {
      return EffectiveHostCompare(a, b);
    };
  };

  
  static const char* GetParseResultString(URLPattern::ParseResult parse_result);

  
  static bool IsSchemeBitSet(const std::string& scheme, const int mask);

 private:
  
  bool MatchesAnyScheme(const std::vector<std::string>& schemes) const;

  
  bool MatchesAllSchemes(const std::vector<std::string>& schemes) const;

  bool MatchesSecurityOriginHelper(const GURL& test) const;

  
  bool MatchesPortPattern(const std::string& port) const;

  
  
  std::vector<std::string> GetExplicitSchemes() const;

  
  
  
  int valid_schemes_;

  
  bool match_all_urls_;

  
  std::string scheme_;

  
  std::string host_;

  
  
  bool match_subdomains_;

  
  std::string port_;

  
  
  std::string path_;

  
  
  std::string path_escaped_;

  
  mutable std::string spec_;
};

typedef std::vector<URLPattern> URLPatternList;

#endif  
