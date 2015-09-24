// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_SERVICE_CLOUD_PRINT_CLOUD_PRINT_URL_FETCHER_H_
#define CHROME_SERVICE_CLOUD_PRINT_CLOUD_PRINT_URL_FETCHER_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "net/url_request/url_fetcher.h"
#include "net/url_request/url_fetcher_delegate.h"

class GURL;

namespace base {
class DictionaryValue;
}

namespace net {
class URLRequestContextGetter;
class URLRequestStatus;
}  

namespace cloud_print {

class CloudPrintURLFetcher;
class CloudPrintURLFetcherFactory {
 public:
  virtual CloudPrintURLFetcher* CreateCloudPrintURLFetcher() = 0;
  virtual ~CloudPrintURLFetcherFactory();
};

class CloudPrintURLFetcher
    : public base::RefCountedThreadSafe<CloudPrintURLFetcher>,
      public net::URLFetcherDelegate {
 public:
  enum ResponseAction {
    CONTINUE_PROCESSING,
    STOP_PROCESSING,
    RETRY_REQUEST,
  };

  enum RequestType {
    REQUEST_AUTH_CODE,
    REQUEST_REGISTER,
    REQUEST_UNREGISTER,
    REQUEST_UPDATE_PRINTER,
    REQUEST_UPDATE_JOB,
    REQUEST_USER_MESSAGE,
    REQUEST_TICKET,
    REQUEST_DATA,
    REQUEST_JOB_FETCH,
    REQUEST_MAX,
  };

  class Delegate {
   public:
    
    
    
    
    virtual ResponseAction HandleRawResponse(
        const net::URLFetcher* source,
        const GURL& url,
        const net::URLRequestStatus& status,
        int response_code,
        const net::ResponseCookies& cookies,
        const std::string& data);

    
    
    
    
    
    
    
    virtual ResponseAction HandleRawData(const net::URLFetcher* source,
                                         const GURL& url,
                                         const std::string& data);

    
    
    
    virtual ResponseAction HandleJSONData(const net::URLFetcher* source,
                                          const GURL& url,
                                          base::DictionaryValue* json_data,
                                          bool succeeded);

    
    
    virtual void OnRequestGiveUp() { }

    
    
    
    
    
    
    virtual ResponseAction OnRequestAuthError() = 0;

    
    
    virtual std::string GetAuthHeader() = 0;

   protected:
    virtual ~Delegate() {}
  };

  static CloudPrintURLFetcher* Create();
  static void set_factory(CloudPrintURLFetcherFactory* factory);

  bool IsSameRequest(const net::URLFetcher* source);

  void StartGetRequest(RequestType type,
                       const GURL& url,
                       Delegate* delegate,
                       int max_retries,
                       const std::string& additional_headers);
  void StartPostRequest(RequestType type,
                        const GURL& url,
                        Delegate* delegate,
                        int max_retries,
                        const std::string& post_data_mime_type,
                        const std::string& post_data,
                        const std::string& additional_headers);

  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

 protected:
  CloudPrintURLFetcher();
  friend class base::RefCountedThreadSafe<CloudPrintURLFetcher>;
  virtual ~CloudPrintURLFetcher();

  
  virtual net::URLRequestContextGetter* GetRequestContextGetter();

 private:
  void StartRequestHelper(RequestType type,
                          const GURL& url,
                          net::URLFetcher::RequestType request_type,
                          Delegate* delegate,
                          int max_retries,
                          const std::string& post_data_mime_type,
                          const std::string& post_data,
                          const std::string& additional_headers);
  void SetupRequestHeaders();
  static CloudPrintURLFetcherFactory* factory();

  scoped_ptr<net::URLFetcher> request_;
  Delegate* delegate_;
  int num_retries_;
  net::URLFetcher::RequestType request_type_;
  std::string additional_headers_;
  std::string post_data_mime_type_;
  std::string post_data_;

  RequestType type_;
  base::Time start_time_;
};

typedef CloudPrintURLFetcher::Delegate CloudPrintURLFetcherDelegate;

}  

#endif  
