// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_URL_REQUEST_FILTER_H_
#define NET_URL_REQUEST_URL_REQUEST_FILTER_H_

#include <map>
#include <string>

#include "base/callback.h"
#include "base/containers/hash_tables.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/net_export.h"
#include "net/url_request/url_request.h"
#include "net/url_request/url_request_job_factory.h"

class GURL;

namespace net {
class URLRequestJob;

class NET_EXPORT URLRequestFilter {
 public:
  
  typedef std::map<std::pair<std::string, std::string>,
      URLRequestJobFactory::ProtocolHandler* > HostnameHandlerMap;
  
  typedef base::hash_map<std::string, URLRequestJobFactory::ProtocolHandler*>
      UrlHandlerMap;

  ~URLRequestFilter();

  static URLRequest::ProtocolFactory Factory;

  
  static URLRequestFilter* GetInstance();

  void AddHostnameHandler(const std::string& scheme,
                          const std::string& hostname,
                          URLRequest::ProtocolFactory* factory);
  void AddHostnameProtocolHandler(
      const std::string& scheme,
      const std::string& hostname,
      scoped_ptr<URLRequestJobFactory::ProtocolHandler> protocol_handler);
  void RemoveHostnameHandler(const std::string& scheme,
                             const std::string& hostname);

  
  
  bool AddUrlHandler(const GURL& url,
                     URLRequest::ProtocolFactory* factory);
  bool AddUrlProtocolHandler(
      const GURL& url,
      scoped_ptr<URLRequestJobFactory::ProtocolHandler> protocol_handler);

  void RemoveUrlHandler(const GURL& url);

  
  
  void ClearHandlers();

  
  int hit_count() const { return hit_count_; }

 protected:
  URLRequestFilter();

  
  URLRequestJob* FindRequestHandler(URLRequest* request,
                                    NetworkDelegate* network_delegate,
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
