// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_GLUE_ALT_ERROR_PAGE_RESOURCE_FETCHER_H_
#define WEBKIT_GLUE_ALT_ERROR_PAGE_RESOURCE_FETCHER_H_

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "googleurl/src/gurl.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebURLError.h"

namespace WebKit {
class WebFrame;
class WebURLResponse;
}

namespace webkit_glue {
class ResourceFetcherWithTimeout;

class AltErrorPageResourceFetcher {
 public:
  
  
  
  typedef Callback3<WebKit::WebFrame*, const WebKit::WebURLError&,
                    const std::string&>::Type Callback;

  AltErrorPageResourceFetcher(const GURL& url,
                              WebKit::WebFrame* frame,
                              const WebKit::WebURLError& original_error,
                              Callback* callback);
  ~AltErrorPageResourceFetcher();

  
  void Cancel();

 private:
  void OnURLFetchComplete(const WebKit::WebURLResponse& response,
                          const std::string& data);

  
  scoped_ptr<ResourceFetcherWithTimeout> fetcher_;

  WebKit::WebFrame* frame_;
  scoped_ptr<Callback> callback_;

  
  
  WebKit::WebURLError original_error_;

  DISALLOW_COPY_AND_ASSIGN(AltErrorPageResourceFetcher);
};

}  

#endif  
