// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_BROWSER_URL_HANDLER_H_
#define CONTENT_PUBLIC_BROWSER_BROWSER_URL_HANDLER_H_

#include "content/common/content_export.h"

class GURL;

namespace content {
class BrowserContext;

class CONTENT_EXPORT BrowserURLHandler {
 public:
  
  
  
  
  typedef bool (*URLHandler)(GURL* url,
                             BrowserContext* browser_context);

  
  static URLHandler null_handler();

  
  static  BrowserURLHandler* GetInstance();

  
  
  
  
  virtual void RewriteURLIfNecessary(GURL* url,
                                     BrowserContext* browser_context,
                                     bool* reverse_on_redirect) = 0;

  
  
  
  
  
  
  virtual void AddHandlerPair(URLHandler handler,
                              URLHandler reverse_handler) = 0;

 protected:
  virtual ~BrowserURLHandler() {}
};

}  

#endif  
