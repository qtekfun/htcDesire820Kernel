// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef URL_THIRD_PARTY_MOZILLA_URL_PARSE_H_
#define URL_THIRD_PARTY_MOZILLA_URL_PARSE_H_

#include <string>

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "url/url_export.h"

namespace url_parse {

typedef base::char16 UTF16Char;


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


struct URL_EXPORT Parsed {
  
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

  
  
  Parsed();
  Parsed(const Parsed&);
  Parsed& operator=(const Parsed&);
  ~Parsed();

  
  
  
  
  
  
  int Length() const;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int CountCharactersBefore(ComponentType type, bool include_delimiter) const;

  
  
  
  
  
  Component scheme;

  
  Component username;

  
  
  
  
  Component password;

  
  Component host;

  
  Component port;

  
  
  
  
  
  
  Component path;

  
  
  
  Component query;

  
  
  
  
  Component ref;

  
  
  
  
  Component GetContent() const;

  
  
  
  
  Parsed* inner_parsed() const {
    return inner_parsed_;
  }

  void set_inner_parsed(const Parsed& inner_parsed) {
    if (!inner_parsed_)
      inner_parsed_ = new Parsed(inner_parsed);
    else
      *inner_parsed_ = inner_parsed;
  }

  void clear_inner_parsed() {
    if (inner_parsed_) {
      delete inner_parsed_;
      inner_parsed_ = NULL;
    }
  }

 private:
  Parsed* inner_parsed_;  
};


URL_EXPORT void ParseStandardURL(const char* url,
                                 int url_len,
                                 Parsed* parsed);
URL_EXPORT void ParseStandardURL(const base::char16* url,
                                 int url_len,
                                 Parsed* parsed);

URL_EXPORT void ParsePathURL(const char* url,
                             int url_len,
                             bool trim_path_end,
                             Parsed* parsed);
URL_EXPORT void ParsePathURL(const base::char16* url,
                             int url_len,
                             bool trim_path_end,
                             Parsed* parsed);

URL_EXPORT void ParseFileURL(const char* url, int url_len, Parsed* parsed);
URL_EXPORT void ParseFileURL(const base::char16* url,
                             int url_len,
                             Parsed* parsed);

URL_EXPORT void ParseFileSystemURL(const char* url,
                                   int url_len,
                                   Parsed* parsed);
URL_EXPORT void ParseFileSystemURL(const base::char16* url,
                                   int url_len,
                                   Parsed* parsed);

URL_EXPORT void ParseMailtoURL(const char* url, int url_len, Parsed* parsed);
URL_EXPORT void ParseMailtoURL(const base::char16* url,
                               int url_len,
                               Parsed* parsed);


URL_EXPORT bool ExtractScheme(const char* url,
                              int url_len,
                              Component* scheme);
URL_EXPORT bool ExtractScheme(const base::char16* url,
                              int url_len,
                              Component* scheme);

URL_EXPORT bool IsAuthorityTerminator(base::char16 ch);

URL_EXPORT void ParseAuthority(const char* spec,
                               const Component& auth,
                               Component* username,
                               Component* password,
                               Component* hostname,
                               Component* port_num);
URL_EXPORT void ParseAuthority(const base::char16* spec,
                               const Component& auth,
                               Component* username,
                               Component* password,
                               Component* hostname,
                               Component* port_num);

enum SpecialPort { PORT_UNSPECIFIED = -1, PORT_INVALID = -2 };
URL_EXPORT int ParsePort(const char* url, const Component& port);
URL_EXPORT int ParsePort(const base::char16* url, const Component& port);

URL_EXPORT void ExtractFileName(const char* url,
                                const Component& path,
                                Component* file_name);
URL_EXPORT void ExtractFileName(const base::char16* url,
                                const Component& path,
                                Component* file_name);

URL_EXPORT bool ExtractQueryKeyValue(const char* url,
                                     Component* query,
                                     Component* key,
                                     Component* value);
URL_EXPORT bool ExtractQueryKeyValue(const base::char16* url,
                                     Component* query,
                                     Component* key,
                                     Component* value);

}  

#endif  
