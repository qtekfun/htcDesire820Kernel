// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ANDROID_WEBVIEW_RENDERER_AW_RENDER_PROCESS_OBSERVER_H_
#define ANDROID_WEBVIEW_RENDERER_AW_RENDER_PROCESS_OBSERVER_H_

#include "content/public/renderer/render_process_observer.h"

#include "base/compiler_specific.h"

namespace android_webview {

class AwRenderProcessObserver : public content::RenderProcessObserver {
 public:
  AwRenderProcessObserver();
  virtual ~AwRenderProcessObserver();

  
  virtual bool OnControlMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void WebKitInitialized() OVERRIDE;

 private:
  void OnClearCache();
  void OnSetJsOnlineProperty(bool network_up);

  bool webkit_initialized_;
};

}  

#endif  

