// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_VARY_DATA_H__
#define NET_HTTP_HTTP_VARY_DATA_H__
#pragma once

#include "base/md5.h"

class Pickle;

namespace net {

struct HttpRequestInfo;
class HttpResponseHeaders;

class HttpVaryData {
 public:
  HttpVaryData();

  bool is_valid() const { return is_valid_; }

  
  
  
  
  
  
  
  
  bool Init(const HttpRequestInfo& request_info,
            const HttpResponseHeaders& response_headers);

  
  
  
  
  
  
  
  bool InitFromPickle(const Pickle& pickle, void** pickle_iter);

  
  
  void Persist(Pickle* pickle) const;

  
  
  
  bool MatchesRequest(const HttpRequestInfo& request_info,
                      const HttpResponseHeaders& cached_response_headers) const;

 private:
  
  static std::string GetRequestValue(const HttpRequestInfo& request_info,
                                     const std::string& request_header);

  
  static void AddField(const HttpRequestInfo& request_info,
                       const std::string& request_header,
                       MD5Context* context);

  
  MD5Digest request_digest_;

  
  bool is_valid_;
};

}  

#endif  
