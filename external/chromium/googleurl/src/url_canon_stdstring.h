// Copyright 2007, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GOOGLEURL_SRC_URL_CANON_STDSTRING_H__
#define GOOGLEURL_SRC_URL_CANON_STDSTRING_H__

#include <string>
#include "googleurl/src/url_canon.h"

namespace url_canon {

// they expect to be written. We will resize if necessary, but that's slow.
class StdStringCanonOutput : public CanonOutput {
 public:
  StdStringCanonOutput(std::string* str)
      : CanonOutput(),
        str_(str) {
    cur_len_ = static_cast<int>(str_->size());  
    str_->resize(str_->capacity());
    buffer_ = &(*str_)[0];
    buffer_len_ = static_cast<int>(str_->size());
  }
  virtual ~StdStringCanonOutput() {
    
  }

  
  void Complete() {
    str_->resize(cur_len_);
    buffer_len_ = cur_len_;
  }

  virtual void Resize(int sz) {
    str_->resize(sz);
    buffer_ = &(*str_)[0];
    buffer_len_ = sz;
  }

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

