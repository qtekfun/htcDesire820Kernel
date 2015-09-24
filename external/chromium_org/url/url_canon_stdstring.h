// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef URL_URL_CANON_STDSTRING_H_
#define URL_URL_CANON_STDSTRING_H_


#include <string>

#include "base/compiler_specific.h"
#include "url/url_canon.h"
#include "url/url_export.h"

namespace url_canon {

// they expect to be written. We will resize if necessary, but that's slow.
class URL_EXPORT StdStringCanonOutput : public CanonOutput {
 public:
  StdStringCanonOutput(std::string* str);
  virtual ~StdStringCanonOutput();

  
  void Complete();

  virtual void Resize(int sz) OVERRIDE;

 protected:
  std::string* str_;
};

template<typename STR>
class StdStringReplacements :
    public url_canon::Replacements<typename STR::value_type> {
 public:
  void SetSchemeStr(const STR& s) {
    this->SetScheme(s.data(),
                    url_parse::Component(0, static_cast<int>(s.length())));
  }
  void SetUsernameStr(const STR& s) {
    this->SetUsername(s.data(),
                      url_parse::Component(0, static_cast<int>(s.length())));
  }
  void SetPasswordStr(const STR& s) {
    this->SetPassword(s.data(),
                      url_parse::Component(0, static_cast<int>(s.length())));
  }
  void SetHostStr(const STR& s) {
    this->SetHost(s.data(),
                  url_parse::Component(0, static_cast<int>(s.length())));
  }
  void SetPortStr(const STR& s) {
    this->SetPort(s.data(),
                  url_parse::Component(0, static_cast<int>(s.length())));
  }
  void SetPathStr(const STR& s) {
    this->SetPath(s.data(),
                  url_parse::Component(0, static_cast<int>(s.length())));
  }
  void SetQueryStr(const STR& s) {
    this->SetQuery(s.data(),
                   url_parse::Component(0, static_cast<int>(s.length())));
  }
  void SetRefStr(const STR& s) {
    this->SetRef(s.data(),
                 url_parse::Component(0, static_cast<int>(s.length())));
  }
};

}  

#endif  
