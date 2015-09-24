// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ANDROID_WEBVIEW_BROWSER_AW_HTTP_AUTH_HANDLER_BASE_H_
#define ANDROID_WEBVIEW_BROWSER_AW_HTTP_AUTH_HANDLER_BASE_H_

namespace content {
class WebContents;
};

namespace net {
class AuthChallengeInfo;
};

namespace android_webview {

class AwLoginDelegate;

class AwHttpAuthHandlerBase {
 public:
  static AwHttpAuthHandlerBase* Create(AwLoginDelegate* login_delegate,
                                       net::AuthChallengeInfo* auth_info,
                                       bool first_auth_attempt);
  virtual ~AwHttpAuthHandlerBase();

  
  
  virtual bool HandleOnUIThread(content::WebContents*) = 0;
};

}  

#endif  
