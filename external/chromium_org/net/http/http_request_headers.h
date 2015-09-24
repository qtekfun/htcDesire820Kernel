// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_REQUEST_HEADERS_H_
#define NET_HTTP_HTTP_REQUEST_HEADERS_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/strings/string_piece.h"
#include "net/base/net_export.h"
#include "net/base/net_log.h"

namespace net {

class NET_EXPORT HttpRequestHeaders {
 public:
  struct HeaderKeyValuePair {
    HeaderKeyValuePair();
    HeaderKeyValuePair(const base::StringPiece& key,
                       const base::StringPiece& value);

    std::string key;
    std::string value;
  };

  typedef std::vector<HeaderKeyValuePair> HeaderVector;

  class NET_EXPORT Iterator {
   public:
    explicit Iterator(const HttpRequestHeaders& headers);
    ~Iterator();

    
    
    
    bool GetNext();

    
    const std::string& name() const { return curr_->key; }
    const std::string& value() const { return curr_->value; }

   private:
    bool started_;
    HttpRequestHeaders::HeaderVector::const_iterator curr_;
    const HttpRequestHeaders::HeaderVector::const_iterator end_;

    DISALLOW_COPY_AND_ASSIGN(Iterator);
  };

  static const char kGetMethod[];

  static const char kAcceptCharset[];
  static const char kAcceptEncoding[];
  static const char kAcceptLanguage[];
  static const char kAuthorization[];
  static const char kCacheControl[];
  static const char kConnection[];
  static const char kContentType[];
  static const char kCookie[];
  static const char kContentLength[];
  static const char kHost[];
  static const char kIfModifiedSince[];
  static const char kIfNoneMatch[];
  static const char kIfRange[];
  static const char kOrigin[];
  static const char kPragma[];
  static const char kProxyAuthorization[];
  static const char kProxyConnection[];
  static const char kRange[];
  static const char kReferer[];
  static const char kUserAgent[];
  static const char kTransferEncoding[];

  HttpRequestHeaders();
  ~HttpRequestHeaders();

  bool IsEmpty() const { return headers_.empty(); }

  bool HasHeader(const base::StringPiece& key) const {
    return FindHeader(key) != headers_.end();
  }

  
  
  bool GetHeader(const base::StringPiece& key, std::string* out) const;

  
  void Clear();

  
  
  
  void SetHeader(const base::StringPiece& key, const base::StringPiece& value);

  
  
  
  void SetHeaderIfMissing(const base::StringPiece& key,
                          const base::StringPiece& value);

  
  void RemoveHeader(const base::StringPiece& key);

  
  
  
  
  
  
  
  
  
  
  
  
  void AddHeaderFromString(const base::StringPiece& header_line);

  
  
  
  void AddHeadersFromString(const base::StringPiece& headers);

  
  void MergeFrom(const HttpRequestHeaders& other);

  
  void CopyFrom(const HttpRequestHeaders& other) {
    *this = other;
  }

  void Swap(HttpRequestHeaders* other) {
    headers_.swap(other->headers_);
  }

  
  
  
  std::string ToString() const;

  
  
  base::Value* NetLogCallback(const std::string* request_line,
                              NetLog::LogLevel log_level) const;

  
  
  
  
  
  static bool FromNetLogParam(const base::Value* event_param,
                              HttpRequestHeaders* headers,
                              std::string* request_line);

 private:
  HeaderVector::iterator FindHeader(const base::StringPiece& key);
  HeaderVector::const_iterator FindHeader(const base::StringPiece& key) const;

  HeaderVector headers_;

  
  
  
  
  
};

}  

#endif  
