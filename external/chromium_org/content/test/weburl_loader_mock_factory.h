// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_TEST_WEBURL_LOADER_MOCK_FACTORY_H_
#define CONTENT_TEST_WEBURL_LOADER_MOCK_FACTORY_H_

#include <map>

#include "base/files/file_path.h"
#include "third_party/WebKit/public/platform/WebURL.h"
#include "third_party/WebKit/public/platform/WebURLError.h"
#include "third_party/WebKit/public/platform/WebURLRequest.h"
#include "third_party/WebKit/public/platform/WebURLResponse.h"

namespace blink {
class WebData;
class WebURLLoader;
}

class WebURLLoaderMock;

class WebURLLoaderMockFactory {
 public:
  WebURLLoaderMockFactory();
  virtual ~WebURLLoaderMockFactory();

  
  
  
  virtual blink::WebURLLoader* CreateURLLoader(
      blink::WebURLLoader* default_loader);

  
  
  void RegisterURL(const blink::WebURL& url,
                   const blink::WebURLResponse& response,
                   const blink::WebString& filePath);

  
  void RegisterErrorURL(const blink::WebURL& url,
                        const blink::WebURLResponse& response,
                        const blink::WebURLError& error);

  
  void UnregisterURL(const blink::WebURL& url);

  
  void UnregisterAllURLs();

  
  void ServeAsynchronousRequests();

  
  blink::WebURLRequest GetLastHandledAsynchronousRequest();

  
  bool IsMockedURL(const blink::WebURL& url);

  
  void LoadSynchronously(const blink::WebURLRequest& request,
                         blink::WebURLResponse* response,
                         blink::WebURLError* error,
                         blink::WebData* data);
  void LoadAsynchronouly(const blink::WebURLRequest& request,
                         WebURLLoaderMock* loader);

  
  void CancelLoad(WebURLLoaderMock* loader);

 private:
  struct ResponseInfo {
    blink::WebURLResponse response;
    base::FilePath file_path;
  };


  
  
  void LoadRequest(const blink::WebURLRequest& request,
                   blink::WebURLResponse* response,
                   blink::WebURLError* error,
                   blink::WebData* data);

  
  bool IsPending(WebURLLoaderMock* loader);

  
  
  static bool ReadFile(const base::FilePath& file_path, blink::WebData* data);

  
  typedef std::map<WebURLLoaderMock*, blink::WebURLRequest> LoaderToRequestMap;
  LoaderToRequestMap pending_loaders_;

  typedef std::map<GURL, blink::WebURLError> URLToErrorMap;
  URLToErrorMap url_to_error_info_;

  
  typedef std::map<GURL, ResponseInfo> URLToResponseMap;
  URLToResponseMap url_to_reponse_info_;

  blink::WebURLRequest last_handled_asynchronous_request_;

  DISALLOW_COPY_AND_ASSIGN(WebURLLoaderMockFactory);
};

#endif  

