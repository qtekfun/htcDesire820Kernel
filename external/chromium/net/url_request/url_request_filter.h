// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_URL_REQUEST_FILTER_H_
#define NET_URL_REQUEST_URL_REQUEST_FILTER_H_
#pragma once

#include <map>
#include <string>

#include "base/hash_tables.h"
#include "net/url_request/url_request.h"

class GURL;

namespace net {
class URLRequestJob;

class URLRequestFilter {
 public:
  
  typedef std::map<std::pair<std::string, std::string>,
      URLRequest::ProtocolFactory*> HostnameHandlerMap;
  typedef base::hash_map<std::string, URLRequest::ProtocolFactory*>
      UrlHandlerMap;

  ~URLRequestFilter();

  static URLRequest::ProtocolFactory Factory;

  
  static URLRequestFilter* GetInstance();

  void AddHostnameHandler(const std::string& scheme,
                          const std::string& hostname,
                          URLRequest::ProtocolFactory* factory);
  void RemoveHostnameHandler(const std::string& scheme,
                             const std::string& hostname);

  
  
  bool AddUrlHandler(const GURL& url,
                     URLRequest::ProtocolFactory* factory);

  void RemoveUrlHandler(const GURL& url);

  
  
  void ClearHandlers();

  
  int hit_count() const { return hit_count_; }

 protected:
  URLRequestFilter();

  
  URLRequestJob* FindRequestHandler(URLRequest* request,
                                    const std::string& scheme);

  
  HostnameHandlerMap hostname_handler_map_;

  
  UrlHandlerMap url_handler_map_;

  int hit_count_;

 private:
  
  static URLRequestFilter* shared_instance_;

  DISALLOW_COPY_AND_ASSIGN(URLRequestFilter);
};

}  

#endif  
