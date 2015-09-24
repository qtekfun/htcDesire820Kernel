// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_VARY_DATA_H_
#define NET_HTTP_HTTP_VARY_DATA_H_

#include "base/md5.h"
#include "net/base/net_export.h"

class Pickle;
class PickleIterator;

namespace net {

struct HttpRequestInfo;
class HttpResponseHeaders;

class NET_EXPORT_PRIVATE HttpVaryData {
 public:
  HttpVaryData();

  bool is_valid() const { return is_valid_; }

  
  
  
  
  
  
  
  
  bool Init(const HttpRequestInfo& request_info,
            const HttpResponseHeaders& response_headers);

  
  
  
  
  
  
  
  bool InitFromPickle(const Pickle& pickle, PickleIterator* pickle_iter);

  
  
  void Persist(Pickle* pickle) const;

  
  
  
  bool MatchesRequest(const HttpRequestInfo& request_info,
                      const HttpResponseHeaders& cached_response_headers) const;

 private:
  
  static std::string GetRequestValue(const HttpRequestInfo& request_info,
                                     const std::string& request_header);

  
  static void AddField(const HttpRequestInfo& request_info,
                       const std::string& request_header,
                       base::MD5Context* context);

  
  base::MD5Digest request_digest_;

  
  bool is_valid_;
};

}  

#endif  
