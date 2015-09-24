// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_URL_UTIL_H_
#define NET_BASE_URL_UTIL_H_

#include <string>

#include "base/compiler_specific.h"
#include "net/base/net_export.h"
#include "url/url_parse.h"

class GURL;

namespace net {

NET_EXPORT GURL AppendQueryParameter(const GURL& url,
                                     const std::string& name,
                                     const std::string& value);

NET_EXPORT GURL AppendOrReplaceQueryParameter(const GURL& url,
                                              const std::string& name,
                                              const std::string& value);

class NET_EXPORT QueryIterator {
 public:
  explicit QueryIterator(const GURL& url);
  ~QueryIterator();

  std::string GetKey() const;
  std::string GetValue() const;
  const std::string& GetUnescapedValue();

  bool IsAtEnd() const;
  void Advance();

 private:
  const GURL& url_;
  url_parse::Component query_;
  bool at_end_;
  url_parse::Component key_;
  url_parse::Component value_;
  std::string unescaped_value_;

  DISALLOW_COPY_AND_ASSIGN(QueryIterator);
};

NET_EXPORT bool GetValueForKeyInQuery(const GURL& url,
                                      const std::string& search_key,
                                      std::string* out_value);

}  

#endif  
