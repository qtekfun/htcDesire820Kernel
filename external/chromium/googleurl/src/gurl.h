// Copyright 2007, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT

#ifndef GOOGLEURL_SRC_GURL_H__
#define GOOGLEURL_SRC_GURL_H__

#include <iosfwd>
#include <string>

#include "base/string16.h"
#include "googleurl/src/url_canon.h"
#include "googleurl/src/url_canon_stdstring.h"
#include "googleurl/src/url_common.h"
#include "googleurl/src/url_parse.h"

class GURL {
 public:
  typedef url_canon::StdStringReplacements<std::string> Replacements;
  typedef url_canon::StdStringReplacements<string16> ReplacementsW;

  
  GURL_API GURL();

  
  
  GURL_API GURL(const GURL& other);

  
  
  
  
  
  
  
  GURL_API explicit GURL(const std::string& url_string
                         );
  GURL_API explicit GURL(const string16& url_string
                         );

  
  
  
  GURL_API GURL(const char* canonical_spec, size_t canonical_spec_len,
                const url_parse::Parsed& parsed, bool is_valid);

  GURL_API GURL& operator=(const GURL& other);

  
  
  
  
  
  bool is_valid() const {
    return is_valid_;
  }

  
  
  
  bool is_empty() const {
    return spec_.empty();
  }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  GURL_API const std::string& spec() const;

  
  
  
  
  
  
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

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  GURL_API GURL Resolve(const std::string& relative) const;
  GURL_API GURL Resolve(const string16& relative) const;

  
  
  
  
  
  
  
  GURL_API GURL ResolveWithCharsetConverter(
      const std::string& relative,
      url_canon::CharsetConverter* charset_converter) const;
  GURL_API GURL ResolveWithCharsetConverter(
      const string16& relative,
      url_canon::CharsetConverter* charset_converter) const;

  
  
  
  
  
  
  
  
  
  
  
  GURL_API GURL ReplaceComponents(
      const url_canon::Replacements<char>& replacements) const;
  GURL_API GURL ReplaceComponents(
      const url_canon::Replacements<char16>& replacements) const;

  
  
  
  
  
  
  
  
  
  GURL_API GURL GetWithEmptyPath() const;

  
  
  
  
  
  
  
  
  
  GURL_API GURL GetOrigin() const;

  
  
  
  
  GURL_API bool IsStandard() const;

  
  
  
  
  GURL_API bool SchemeIs(const char* lower_ascii_scheme) const;

  
  
  bool SchemeIsFile() const {
    return SchemeIs("file");
  }

  
  bool SchemeIsSecure() const {
    return SchemeIs("https");
  }

  
  
  
  GURL_API bool HostIsIPAddress() const;

  
  
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

  
  
  GURL_API int IntPort() const;

  
  
  
  GURL_API int EffectiveIntPort() const;

  
  
  GURL_API std::string ExtractFileName() const;

  
  
  GURL_API std::string PathForRequest() const;

  
  
  GURL_API std::string HostNoBrackets() const;

  
  
  
  
  
  
  
  
  
  
  GURL_API bool DomainIs(const char* lower_ascii_domain, int domain_len) const;

  
  
  bool DomainIs(const char* lower_ascii_domain) const {
    return DomainIs(lower_ascii_domain,
                    static_cast<int>(strlen(lower_ascii_domain)));
  }

  
  
  GURL_API void Swap(GURL* other);

  
  
  
  GURL_API static const GURL& EmptyGURL();

 private:
  
  std::string ComponentString(const url_parse::Component& comp) const {
    if (comp.len <= 0)
      return std::string();
    return std::string(spec_, comp.begin, comp.len);
  }

  
  std::string spec_;

  
  
  
  bool is_valid_;

  
  url_parse::Parsed parsed_;

  
};

GURL_API std::ostream& operator<<(std::ostream& out, const GURL& url);

#endif  
