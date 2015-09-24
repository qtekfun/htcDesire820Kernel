// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_PROXY_PROXY_SCRIPT_FETCHER_H_
#define NET_PROXY_PROXY_SCRIPT_FETCHER_H_
#pragma once

#include "base/string16.h"
#include "net/base/completion_callback.h"

class GURL;

namespace net {

class URLRequestContext;

class ProxyScriptFetcher {
 public:
  
  virtual ~ProxyScriptFetcher() {}

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual int Fetch(const GURL& url, string16* utf16_text,
                    CompletionCallback* callback) = 0;

  
  virtual void Cancel() = 0;

  
  
  virtual URLRequestContext* GetRequestContext() = 0;
};

}  

#endif  
