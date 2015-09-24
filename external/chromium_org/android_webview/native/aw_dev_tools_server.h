// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ANDROID_WEBVIEW_NATIVE_AW_DEV_TOOLS_SERVER_H_
#define ANDROID_WEBVIEW_NATIVE_AW_DEV_TOOLS_SERVER_H_

#include <jni.h>
#include <vector>

#include "base/basictypes.h"

namespace content {
class DevToolsHttpHandler;
}

namespace android_webview {

class AwDevToolsServer {
 public:
  AwDevToolsServer();
  ~AwDevToolsServer();

  
  void Start();

  
  void Stop();

  bool IsStarted() const;

 private:
  content::DevToolsHttpHandler* protocol_handler_;

  DISALLOW_COPY_AND_ASSIGN(AwDevToolsServer);
};

bool RegisterAwDevToolsServer(JNIEnv* env);

}  

#endif  
