// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef URL_GURL_H_
#define URL_GURL_H_

#include <iosfwd>
#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "url/url_canon.h"
#include "url/url_canon_stdstring.h"
#include "url/url_export.h"
#include "url/url_parse.h"

class URL_EXPORT GURL {
 public:
  typedef url_canon::StdStringReplacements<std::string> Replacements;
  typedef url_canon::StdStringReplacements<base::string16> ReplacementsW;

  
  GURL();

  
  
  GURL(const GURL& other);

  
  
  
  
  
  
  
  explicit GURL(const std::string& url_string );
  explicit GURL(const base::string16& url_string );

  
  
  
  GURL(const char* canonical_spec, size_t canonical_spec_len,
       const url_parse::Parsed& parsed, bool is_valid);
  
  
  
  
  
  
  GURL(std::string canonical_spec,
       const url_parse::Parsed& parsed, bool is_valid);

  ~GURL();

  GURL& operator=(GURL other);

  
  
  
  
  
  bool is_valid() const {
    return is_valid_;
  }

  
  
  
  bool is_empty() const {
    return spec_.empty();
  }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  const std::string& spec() const;

  
  
  
  
  
  
  const std::string& possibly_invalid_spec() const {
    return spec_;
  }

  
  
  
  
  
  
  
  
  const url_parse::Parsed& parsed_for_possibly_invalid_spec() const {
    return parsed_;
  }

  
  bool operator==(const GURL& other) const {
    return spec_ == other.spec_;
  }
  bool operator!=(const GURL& other) const {
    return spec_ != other.spec_;
  }

  
  bool operator<(const GURL& other) const {
    return spec_ < other.spec_;
  }
  bool operator>(const GURL& other) const {
    return spec_ > other.spec_;
  }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  GURL Resolve(const std::string& relative) const;
  GURL Resolve(const base::string16& relative) const;

  
  
  
  
  
  
  
  GURL ResolveWithCharsetConverter(
      const std::string& relative,
      url_canon::CharsetConverter* charset_converter) const;
  GURL ResolveWithCharsetConverter(
      const base::string16& relative,
      url_canon::CharsetConverter* charset_converter) const;

  
  
  
  
  
  
  
  
  
  
  
  GURL ReplaceComponents(
      const url_canon::Replacements<char>& replacements) const;
  GURL ReplaceComponents(
      const url_canon::Replacements<base::char16>& replacements) const;

  
  
  
  
  
  
  
  
  
  GURL GetWithEmptyPath() const;

  
  
  
  
  
  
  
  
  
  GURL GetOrigin() const;

  
  
  
  
  bool IsStandard() const;

  
  
  
  
  bool SchemeIs(const char* lower_ascii_scheme) const;

  
  bool SchemeIsHTTPOrHTTPS() const;

  
  bool SchemeIsWSOrWSS() const;

  
  
  bool SchemeIsFile() const {
    return SchemeIs("file");
  }

  
  bool SchemeIsFileSystem() const {
    return SchemeIs("filesystem");
  }

  
  bool SchemeIsSecure() const {
    return SchemeIs("https") || SchemeIs("wss") ||
        (SchemeIsFileSystem() && inner_url() && inner_url()->SchemeIsSecure());
  }

  
  
  
  std::string GetContent() const;

  
  
  
  bool HostIsIPAddress() const;

  
  
  std::string scheme() const {  
    return ComponentString(parsed_.scheme);
  }
  std::string username() const {
    return ComponentString(parsed_.username);
  }
  std::string password() const {
    return ComponentString(parsed_.password);
  }
  
  
  
  std::string host() const {
    return ComponentString(parsed_.host);
  }
  std::string port() const {  
    return ComponentString(parsed_.port);
  }
  std::string path() const {  
    return ComponentString(parsed_.path);
  }
  std::string query() const {  
    return ComponentString(parsed_.query);
  }
  std::string ref() const {  
    return ComponentString(parsed_.ref);
  }

  
  
  
  
  bool has_scheme() const {
    return parsed_.scheme.len >= 0;
  }
  bool has_username() const {
    return parsed_.username.len >= 0;
  }
  bool has_password() const {
    return parsed_.password.len >= 0;
  }
  bool has_host() const {
    
    return parsed_.host.len > 0;
  }
  bool has_port() const {
    return parsed_.port.len >= 0;
  }
  bool has_path() const {
    
    
    return parsed_.path.len >= 0;
  }
  bool has_query() const {
    return parsed_.query.len >= 0;
  }
  bool has_ref() const {
    return parsed_.ref.len >= 0;
  }

  
  
  int IntPort() const;

  
  
  
  int EffectiveIntPort() const;

  
  
  std::string ExtractFileName() const;

  
  
  std::string PathForRequest() const;

  
  
  std::string HostNoBrackets() const;

  
  
  
  
  
  
  
  
  
  
  bool DomainIs(const char* lower_ascii_domain, int domain_len) const;

  
  
  bool DomainIs(const char* lower_ascii_domain) const {
    return DomainIs(lower_ascii_domain,
                    static_cast<int>(strlen(lower_ascii_domain)));
  }

  
  
  void Swap(GURL* other);

  
  
  
  static const GURL& EmptyGURL();

  
  
  const GURL* inner_url() const {
    return inner_url_.get();
  }

 private:
  
  
  
  
  
  enum RetainWhiteSpaceSelector { RETAIN_TRAILING_PATH_WHITEPACE };
  GURL(const std::string& url_string, RetainWhiteSpaceSelector);

  template<typename STR>
  void InitCanonical(const STR& input_spec, bool trim_path_end);

  void InitializeFromCanonicalSpec();

  
  std::string ComponentString(const url_parse::Component& comp) const {
    if (comp.len <= 0)
      return std::string();
    return std::string(spec_, comp.begin, comp.len);
  }

  
  std::string spec_;

  
  
  
  bool is_valid_;

  
  url_parse::Parsed parsed_;

  
  scoped_ptr<GURL> inner_url_;

  
};

URL_EXPORT std::ostream& operator<<(std::ostream& out, const GURL& url);

#endif  
