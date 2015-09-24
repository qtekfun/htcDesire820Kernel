// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_DEVTOOLS_DEVTOOLS_CLIENT_H_
#define CONTENT_RENDERER_DEVTOOLS_DEVTOOLS_CLIENT_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "content/public/renderer/render_view_observer.h"
#include "third_party/WebKit/public/web/WebDevToolsFrontendClient.h"

namespace blink {
class WebDevToolsFrontend;
class WebString;
}

namespace content {

class RenderViewImpl;

class CONTENT_EXPORT DevToolsClient
  : public RenderViewObserver,
    NON_EXPORTED_BASE(public blink::WebDevToolsFrontendClient) {
 public:
  explicit DevToolsClient(RenderViewImpl* render_view);
  virtual ~DevToolsClient();

 private:
  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  
  virtual void sendMessageToBackend(const blink::WebString&) OVERRIDE;
  virtual void sendMessageToEmbedder(const blink::WebString&) OVERRIDE;

  virtual bool isUnderTest() OVERRIDE;

  void OnDispatchOnInspectorFrontend(const std::string& message);

  scoped_ptr<blink::WebDevToolsFrontend> web_tools_frontend_;

  DISALLOW_COPY_AND_ASSIGN(DevToolsClient);
};

}  

#endif  
