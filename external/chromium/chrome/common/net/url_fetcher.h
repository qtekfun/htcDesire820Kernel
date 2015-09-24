// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_COMMON_NET_URL_FETCHER_H_
#define CHROME_COMMON_NET_URL_FETCHER_H_
#pragma once

#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/message_loop.h"
#include "base/time.h"

class GURL;
typedef std::vector<std::string> ResponseCookies;

namespace net {
class HttpResponseHeaders;
class URLRequestContextGetter;
class URLRequestStatus;
}  


class URLFetcher {
 public:
  enum RequestType {
    GET,
    POST,
    HEAD,
  };

  class Delegate {
   public:
    
    
    
    
    virtual void OnURLFetchComplete(const URLFetcher* source,
                                    const GURL& url,
                                    const net::URLRequestStatus& status,
                                    int response_code,
                                    const ResponseCookies& cookies,
                                    const std::string& data) = 0;

   protected:
    virtual ~Delegate() {}
  };

  
  
  class Factory {
   public:
    virtual URLFetcher* CreateURLFetcher(int id,
                                         const GURL& url,
                                         RequestType request_type,
                                         Delegate* d) = 0;

   protected:
    virtual ~Factory() {}
  };

  
  
  
  URLFetcher(const GURL& url, RequestType request_type, Delegate* d);

  virtual ~URLFetcher();

  
  
  
#if defined(UNIT_TEST)
  static void set_factory(Factory* factory) { factory_ = factory; }
#endif

  
  
  
  static void enable_interception_for_tests(bool enabled) {
    g_interception_enabled = enabled;
  }

  
  
  
  
  static URLFetcher* Create(int id, const GURL& url, RequestType request_type,
                            Delegate* d);

  
  
  
  
#ifdef ANDROID
  
  virtual
#endif
  void set_upload_data(const std::string& upload_content_type,
                       const std::string& upload_content);

  
  
  
  void set_chunked_upload(const std::string& upload_content_type);

  
  
  
  virtual void AppendChunkToUpload(const std::string& data, bool is_last_chunk);

  
  
  void set_load_flags(int load_flags);

  
  int load_flags() const;

  
  
  void set_referrer(const std::string& referrer);

  
  
  void set_extra_request_headers(const std::string& extra_request_headers);

  
  
#ifdef ANDROID
  
  virtual
#endif
  void set_request_context(
      net::URLRequestContextGetter* request_context_getter);

#ifdef ANDROID
  net::URLRequestContextGetter* request_context();
#endif

  
  
  
#ifdef ANDROID
  
  virtual
#endif
  void set_automatically_retry_on_5xx(bool retry);

  int max_retries() const { return max_retries_; }

  void set_max_retries(int max_retries) { max_retries_ = max_retries; }

  
  
  base::TimeDelta backoff_delay() const { return backoff_delay_; }

  
#if defined(UNIT_TEST)
  void set_backoff_delay(base::TimeDelta backoff_delay) {
    backoff_delay_ = backoff_delay;
  }
#endif  

  
  
  virtual net::HttpResponseHeaders* response_headers() const;

  
  
  virtual void Start();

  
  const GURL& url() const;

  
  void ReceivedContentWasMalformed();

  
  
  
  
  
  
  static void CancelAll();

 protected:
  
  Delegate* delegate() const;

  
  const std::string& upload_data() const;

 private:
  friend class URLFetcherTest;

  
  
  static int GetNumFetcherCores();

  class Core;

  scoped_refptr<Core> core_;

  static Factory* factory_;

  
  
  
  
  bool automatically_retry_on_5xx_;
  
  base::TimeDelta backoff_delay_;
  
  int max_retries_;

  static bool g_interception_enabled;

  DISALLOW_COPY_AND_ASSIGN(URLFetcher);
};

#endif  
