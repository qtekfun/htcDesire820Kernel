// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ANDROID_WEBVIEW_BROWSER_INTERCEPTED_REQUEST_DATA_H_
#define ANDROID_WEBVIEW_BROWSER_INTERCEPTED_REQUEST_DATA_H_

#include <string>

#include "base/android/jni_android.h"
#include "base/memory/scoped_ptr.h"

namespace net {
class URLRequest;
class URLRequestJob;
class NetworkDelegate;
}

namespace android_webview {

class InputStream;

class InterceptedRequestData {
 public:
  virtual ~InterceptedRequestData() {}

  virtual scoped_ptr<InputStream> GetInputStream(JNIEnv* env) const = 0;
  virtual bool GetMimeType(JNIEnv* env, std::string* mime_type) const = 0;
  virtual bool GetCharset(JNIEnv* env, std::string* charset) const = 0;

  
  
  
  
  static net::URLRequestJob* CreateJobFor(
      scoped_ptr<InterceptedRequestData> intercepted_request_data,
      net::URLRequest* request,
      net::NetworkDelegate* network_delegate);

 protected:
  InterceptedRequestData() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(InterceptedRequestData);
};

} 

#endif  
