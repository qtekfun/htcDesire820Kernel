// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_FETCHERS_ALT_ERROR_PAGE_RESOURCE_FETCHER_H_
#define CONTENT_RENDERER_FETCHERS_ALT_ERROR_PAGE_RESOURCE_FETCHER_H_

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "third_party/WebKit/public/platform/WebURLError.h"
#include "third_party/WebKit/public/platform/WebURLRequest.h"
#include "url/gurl.h"

namespace blink {
class WebFrame;
class WebURLResponse;
}

namespace content {
class ResourceFetcher;

class AltErrorPageResourceFetcher {
 public:
  
  
  
  typedef base::Callback<void(blink::WebFrame*,
                              const blink::WebURLRequest&,
                              const blink::WebURLError&,
                              const std::string&)> Callback;

  AltErrorPageResourceFetcher(
      const GURL& url,
      blink::WebFrame* frame,
      const blink::WebURLRequest& original_request,
      const blink::WebURLError& original_error,
      const Callback& callback);
  ~AltErrorPageResourceFetcher();

 private:
  void OnURLFetchComplete(const blink::WebURLResponse& response,
                          const std::string& data);

  
  scoped_ptr<ResourceFetcher> fetcher_;

  blink::WebFrame* frame_;
  Callback callback_;

  
  
  blink::WebURLRequest original_request_;

  
  
  blink::WebURLError original_error_;

  DISALLOW_COPY_AND_ASSIGN(AltErrorPageResourceFetcher);
};

}  

#endif  
