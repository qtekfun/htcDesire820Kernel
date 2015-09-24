// Copyright 2007, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT

#ifndef GOOGLEURL_SRC_URL_PARSE_H__
#define GOOGLEURL_SRC_URL_PARSE_H__

#include <string>

#include "base/basictypes.h"
#include "base/string16.h"
#include "googleurl/src/url_common.h"

namespace url_parse {

typedef char16 UTF16Char;


struct Component {
  Component() : begin(0), len(-1) {}

  
  Component(int b, int l) : begin(b), len(l) {}

  int end() const {
    return begin + len;
  }

  
  
  bool is_valid() const {
    return (len != -1);
  }

  
  
  bool is_nonempty() const {
    return (len > 0);
  }

  void reset() {
    begin = 0;
    len = -1;
  }

  bool operator==(const Component& other) const {
    return begin == other.begin && len == other.len;
  }

  int begin;  
  int len;    
};

inline Component MakeRange(int begin, int end) {
  return Component(begin, end - begin);
}


struct Parsed {
  
  enum ComponentType {
    SCHEME,
    USERNAME,
    PASSWORD,
    HOST,
    PORT,
    PATH,
    QUERY,
    REF,
  };

  
  GURL_API Parsed();

  
  
  
  
  
  
  GURL_API int Length() const;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  GURL_API int CountCharactersBefore(ComponentType type,
                                     bool include_delimiter) const;

  
  
  
  
  
  Component scheme;

  
  Component username;

  
  
  
  
  Component password;

  
  Component host;

  
  Component port;

  
  
  
  
  
  Component path;

  
  
  
  Component query;

  
  
  
  
  Component ref;
};


GURL_API void ParseStandardURL(const char* url, int url_len, Parsed* parsed);
GURL_API void ParseStandardURL(const char16* url, int url_len, Parsed* parsed);

GURL_API void ParsePathURL(const char* url, int url_len, Parsed* parsed);
GURL_API void ParsePathURL(const char16* url, int url_len, Parsed* parsed);

GURL_API void ParseFileURL(const char* url, int url_len, Parsed* parsed);
GURL_API void ParseFileURL(const char16* url, int url_len, Parsed* parsed);

GURL_API void ParseMailtoURL(const char* url, int url_len, Parsed* parsed);
GURL_API void ParseMailtoURL(const char16* url, int url_len, Parsed* parsed);


GURL_API bool ExtractScheme(const char* url, int url_len, Component* scheme);
GURL_API bool ExtractScheme(const char16* url, int url_len, Component* scheme);

GURL_API bool IsAuthorityTerminator(char16 ch);

GURL_API void ParseAuthority(const char* spec,
                             const Component& auth,
                             Component* username,
                             Component* password,
                             Component* hostname,
                             Component* port_num);
GURL_API void ParseAuthority(const char16* spec,
                             const Component& auth,
                             Component* username,
                             Component* password,
                             Component* hostname,
                             Component* port_num);

enum SpecialPort { PORT_UNSPECIFIED = -1, PORT_INVALID = -2 };
GURL_API int ParsePort(const char* url, const Component& port);
GURL_API int ParsePort(const char16* url, const Component& port);

GURL_API void ExtractFileName(const char* url,
                              const Component& path,
                              Component* file_name);
GURL_API void ExtractFileName(const char16* url,
                              const Component& path,
                              Component* file_name);

GURL_API bool ExtractQueryKeyValue(const char* url,
                                   Component* query,
                                   Component* key,
                                   Component* value);
GURL_API bool ExtractQueryKeyValue(const char16* url,
                                   Component* query,
                                   Component* key,
                                   Component* value);

}  

#endif  
