// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_WEB_REQUEST_FORM_DATA_PARSER_H_
#define CHROME_BROWSER_EXTENSIONS_API_WEB_REQUEST_FORM_DATA_PARSER_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string_piece.h"

namespace net {
class URLRequest;
}

namespace extensions {

class FormDataParser {
 public:
  class Result {
   public:
    Result();
    ~Result();
    const std::string& name() const {
      return name_;
    }
    const std::string& value() const {
      return value_;
    }
    void set_value(const base::StringPiece& str) {
      str.CopyToString(&value_);
    }
    void set_name(const std::string& str) {
      name_ = str;
    }
    void set_value(const std::string& str) {
      value_ = str;
    }

   private:
    std::string name_;
    std::string value_;

    DISALLOW_COPY_AND_ASSIGN(Result);
  };

  virtual ~FormDataParser();

  
  
  static scoped_ptr<FormDataParser> Create(const net::URLRequest& request);

  
  
  
  static scoped_ptr<FormDataParser> CreateFromContentTypeHeader(
      const std::string* content_type_header);

  
  virtual bool AllDataReadOK() = 0;

  
  
  
  virtual bool GetNextNameValue(Result* result) = 0;

  
  
  
  virtual bool SetSource(const base::StringPiece& source) = 0;

 protected:
  FormDataParser();

 private:
  DISALLOW_COPY_AND_ASSIGN(FormDataParser);
};

}  

#endif  
