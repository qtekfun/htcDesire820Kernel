// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_GLUE_FTP_DIRECTORY_LISTING_RESPONSE_DELEGATE_H_
#define WEBKIT_GLUE_FTP_DIRECTORY_LISTING_RESPONSE_DELEGATE_H_

#include <string>

#include "base/basictypes.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebURLResponse.h"

namespace WebKit {
class WebURLLoader;
class WebURLLoaderClient;
}

class GURL;

namespace webkit_glue {

class FtpDirectoryListingResponseDelegate {
 public:
  FtpDirectoryListingResponseDelegate(WebKit::WebURLLoaderClient* client,
                                      WebKit::WebURLLoader* loader,
                                      const WebKit::WebURLResponse& response);

  
  void OnReceivedData(const char* data, int data_len);
  void OnCompletedRequest();

 private:
  void Init(const GURL& response_url);

  void SendDataToClient(const std::string& data);

  
  
  WebKit::WebURLLoaderClient* client_;
  WebKit::WebURLLoader* loader_;

  
  std::string buffer_;

  DISALLOW_COPY_AND_ASSIGN(FtpDirectoryListingResponseDelegate);
};

}  

#endif  
