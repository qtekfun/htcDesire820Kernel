// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_BROWSER_URL_HANDLER_IMPL_H_
#define CONTENT_BROWSER_BROWSER_URL_HANDLER_IMPL_H_

#include <vector>
#include <utility>

#include "base/gtest_prod_util.h"
#include "base/memory/singleton.h"
#include "content/public/browser/browser_url_handler.h"

class GURL;

namespace content {
class BrowserContext;

class CONTENT_EXPORT BrowserURLHandlerImpl : public BrowserURLHandler {
 public:
  
  static BrowserURLHandlerImpl* GetInstance();

  
  virtual void RewriteURLIfNecessary(GURL* url,
                                     BrowserContext* browser_context,
                                     bool* reverse_on_redirect) OVERRIDE;
  
  virtual void AddHandlerPair(URLHandler handler,
                              URLHandler reverse_handler) OVERRIDE;

  
  bool ReverseURLRewrite(GURL* url, const GURL& original,
                         BrowserContext* browser_context);

 private:
  
  BrowserURLHandlerImpl();
  virtual ~BrowserURLHandlerImpl();
  friend struct DefaultSingletonTraits<BrowserURLHandlerImpl>;

  
  typedef std::pair<URLHandler, URLHandler> HandlerPair;
  std::vector<HandlerPair> url_handlers_;

  FRIEND_TEST_ALL_PREFIXES(BrowserURLHandlerImplTest, BasicRewriteAndReverse);
  FRIEND_TEST_ALL_PREFIXES(BrowserURLHandlerImplTest, NullHandlerReverse);

  DISALLOW_COPY_AND_ASSIGN(BrowserURLHandlerImpl);
};

}  

#endif  
