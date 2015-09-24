// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_CONTENT_DISPOSITION_H_
#define NET_HTTP_HTTP_CONTENT_DISPOSITION_H_

#include <string>

#include "base/basictypes.h"
#include "net/base/net_export.h"

namespace net {

class NET_EXPORT HttpContentDisposition {
 public:
  enum Type {
    INLINE,
    ATTACHMENT,
  };

  
  enum ParseResultFlags {
    INVALID                      = 0,

    
    HAS_DISPOSITION_TYPE         = 1 << 0,

    
    HAS_UNKNOWN_DISPOSITION_TYPE = 1 << 1,

    
    HAS_NAME                     = 1 << 2,

    
    HAS_FILENAME                 = 1 << 3,

    
    HAS_EXT_FILENAME             = 1 << 4,

    

    
    HAS_NON_ASCII_STRINGS        = 1 << 5,

    
    HAS_PERCENT_ENCODED_STRINGS  = 1 << 6,

    
    HAS_RFC2047_ENCODED_STRINGS  = 1 << 7
  };

  HttpContentDisposition(const std::string& header,
                         const std::string& referrer_charset);
  ~HttpContentDisposition();

  bool is_attachment() const { return type() == ATTACHMENT; }

  Type type() const { return type_; }
  const std::string& filename() const { return filename_; }

  
  int parse_result_flags() const { return parse_result_flags_; }

 private:
  void Parse(const std::string& header, const std::string& referrer_charset);
  std::string::const_iterator ConsumeDispositionType(
      std::string::const_iterator begin, std::string::const_iterator end);

  Type type_;
  std::string filename_;
  int parse_result_flags_;

  DISALLOW_COPY_AND_ASSIGN(HttpContentDisposition);
};

}  

#endif  
