// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_RENDERER_RESOURCE_FETCHER_H_
#define CONTENT_PUBLIC_RENDERER_RESOURCE_FETCHER_H_

#include <string>

#include "base/callback.h"
#include "content/common/content_export.h"
#include "third_party/WebKit/public/platform/WebURLRequest.h"

class GURL;

namespace base {
class TimeDelta;
}

namespace blink {
class WebFrame;
class WebURLResponse;
}

namespace content {

class CONTENT_EXPORT ResourceFetcher {
 public:
  virtual ~ResourceFetcher() {}

  
  
  
  
  typedef base::Callback<void(const blink::WebURLResponse& response,
                              const std::string& data)> Callback;

  
  
  
  
  static ResourceFetcher* Create(const GURL& url,
                                 blink::WebFrame* frame,
                                 blink::WebURLRequest::TargetType target_type,
                                 const Callback& callback);

  
  
  virtual void SetTimeout(const base::TimeDelta& timeout) = 0;
};

}  

#endif  
