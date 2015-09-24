// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ANDROID_WEBVIEW_NATIVE_ANDROID_PROTOCOL_HANDLER_H_
#define ANDROID_WEBVIEW_NATIVE_ANDROID_PROTOCOL_HANDLER_H_

#include "base/android/jni_android.h"
#include "base/memory/scoped_ptr.h"
#include "net/url_request/url_request_job_factory.h"

namespace net {
class URLRequestContext;
}  

namespace android_webview {

scoped_ptr<net::URLRequestJobFactory::ProtocolHandler>
    CreateContentSchemeProtocolHandler();

scoped_ptr<net::URLRequestJobFactory::ProtocolHandler>
    CreateAssetFileProtocolHandler();

bool RegisterAndroidProtocolHandler(JNIEnv* env);

}  

#endif  
