// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_PROXY_PROXY_SCRIPT_FETCHER_H_
#define NET_PROXY_PROXY_SCRIPT_FETCHER_H_

#include "base/strings/string16.h"
#include "net/base/completion_callback.h"
#include "net/base/net_export.h"

class GURL;

namespace net {

class URLRequestContext;

class NET_EXPORT_PRIVATE ProxyScriptFetcher {
 public:
  
  virtual ~ProxyScriptFetcher() {}

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual int Fetch(const GURL& url, base::string16* utf16_text,
                    const net::CompletionCallback& callback) = 0;

  
  virtual void Cancel() = 0;

  
  
  virtual URLRequestContext* GetRequestContext() const = 0;
};

}  

#endif  
