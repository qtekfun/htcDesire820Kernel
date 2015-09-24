// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_DNS_BLACKHOLE_CHECKER_H_
#define REMOTING_HOST_DNS_BLACKHOLE_CHECKER_H_

#include "net/url_request/url_fetcher_delegate.h"

#include "base/callback.h"

namespace net {
class URLRequestContextGetter;
}  

namespace remoting {

extern const char kDefaultHostTalkGadgetPrefix[];

class DnsBlackholeChecker : public net::URLFetcherDelegate {
 public:
  DnsBlackholeChecker(
      scoped_refptr<net::URLRequestContextGetter> url_request_context_getter,
      std::string talkgadget_prefix);
  virtual ~DnsBlackholeChecker();

  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

  
  
  
  void CheckForDnsBlackhole(const base::Callback<void(bool)>& callback);

 private:
  
  scoped_refptr<net::URLRequestContextGetter> url_request_context_getter_;

  
  scoped_ptr<net::URLFetcher> url_fetcher_;

  
  
  std::string talkgadget_prefix_;

  
  base::Callback<void(bool)> callback_;

  DISALLOW_COPY_AND_ASSIGN(DnsBlackholeChecker);
};

}  

#endif  
