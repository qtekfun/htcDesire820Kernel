// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_GLUE_RESOURCE_FETCHER_H_
#define WEBKIT_GLUE_RESOURCE_FETCHER_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/timer.h"
#include "googleurl/src/gurl.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebURLLoaderClient.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebURLRequest.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebURLResponse.h"

class GURL;

namespace WebKit {
class WebFrame;
class WebURLLoader;
struct WebURLError;
}

namespace webkit_glue {

class ResourceFetcher : public WebKit::WebURLLoaderClient {
 public:
  
  
  
  typedef Callback2<const WebKit::WebURLResponse&,
                    const std::string&>::Type Callback;

  
  ResourceFetcher(
      const GURL& url, WebKit::WebFrame* frame,
      WebKit::WebURLRequest::TargetType target_type, Callback* callback);
  ~ResourceFetcher();

  
  void Cancel();

  bool completed() const { return completed_; }

 protected:
  
  virtual void willSendRequest(
      WebKit::WebURLLoader* loader, WebKit::WebURLRequest& new_request,
      const WebKit::WebURLResponse& redirect_response);
  virtual void didSendData(
      WebKit::WebURLLoader* loader, unsigned long long bytes_sent,
      unsigned long long total_bytes_to_be_sent);
  virtual void didReceiveResponse(
      WebKit::WebURLLoader* loader, const WebKit::WebURLResponse& response);
  virtual void didReceiveCachedMetadata(
      WebKit::WebURLLoader* loader, const char* data, int data_length);

  virtual void didReceiveData(
      WebKit::WebURLLoader* loader, const char* data, int data_length,
      int encoded_data_length);
  virtual void didFinishLoading(
      WebKit::WebURLLoader* loader, double finishTime);
  virtual void didFail(
      WebKit::WebURLLoader* loader, const WebKit::WebURLError& error);

  scoped_ptr<WebKit::WebURLLoader> loader_;

  
  GURL url_;

  
  WebKit::WebURLRequest::TargetType target_type_;

  
  WebKit::WebURLResponse response_;

  
  bool completed_;

 private:
  
  void Start(WebKit::WebFrame* frame);

  void RunCallback(const WebKit::WebURLResponse& response,
                   const std::string& data);

  
  scoped_ptr<Callback> callback_;

  
  std::string data_;

  
  std::string metadata_;
};

class ResourceFetcherWithTimeout : public ResourceFetcher {
 public:
  ResourceFetcherWithTimeout(const GURL& url,
                             WebKit::WebFrame* frame,
                             WebKit::WebURLRequest::TargetType target_type,
                             int timeout_secs,
                             Callback* callback);
  virtual ~ResourceFetcherWithTimeout();

 private:
  
  
  
  void TimeoutFired();

  
  base::OneShotTimer<ResourceFetcherWithTimeout> timeout_timer_;
};

}  

#endif  
