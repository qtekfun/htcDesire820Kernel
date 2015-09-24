// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_REMOTING_DIRECTORY_ADD_REQUEST_H_
#define CHROME_BROWSER_REMOTING_DIRECTORY_ADD_REQUEST_H_

#include <string>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/common/remoting/chromoting_host_info.h"
#include "chrome/common/net/url_fetcher.h"
#include "googleurl/src/gurl.h"

namespace remoting {

class DirectoryAddRequest : public URLFetcher::Delegate {
 public:
  enum Result {
    
    SUCCESS,
    
    ERROR_AUTH,
    
    
    ERROR_INVALID_REQUEST,
    
    ERROR_EXISTS,
    
    ERROR_SERVER,
    
    ERROR_TIMEOUT_EXPIRED,
    
    ERROR_OTHER,
  };

  
  
  
  
  typedef Callback2<Result, const std::string&>::Type DoneCallback;

  explicit DirectoryAddRequest(net::URLRequestContextGetter* getter);
  ~DirectoryAddRequest();

  
  
  
  void AddHost(const remoting::ChromotingHostInfo& host_info,
               const std::string& auth_token,
               DoneCallback* done_callback);

  
  virtual void OnURLFetchComplete(const URLFetcher* source,
                                  const GURL& url,
                                  const net::URLRequestStatus& status,
                                  int response_code,
                                  const ResponseCookies& cookies,
                                  const std::string& data);

 private:
  friend class DirectoryAddRequestTest;

  net::URLRequestContextGetter* getter_;
  scoped_ptr<DoneCallback> done_callback_;
  scoped_ptr<URLFetcher> fetcher_;

  DISALLOW_COPY_AND_ASSIGN(DirectoryAddRequest);
};

}  

#endif  
