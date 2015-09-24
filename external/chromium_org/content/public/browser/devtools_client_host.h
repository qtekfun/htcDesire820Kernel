// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_DEVTOOLS_CLIENT_HOST_H_
#define CONTENT_PUBLIC_BROWSER_DEVTOOLS_CLIENT_HOST_H_

#include <string>

#include "base/basictypes.h"
#include "content/common/content_export.h"

namespace IPC {
class Message;
}

namespace content {

class RenderViewHost;
class WebContents;

class DevToolsFrontendHostDelegate;

class CONTENT_EXPORT DevToolsClientHost {
 public:
  virtual ~DevToolsClientHost() {}

  
  virtual void DispatchOnInspectorFrontend(const std::string& message) = 0;

  
  
  virtual void InspectedContentsClosing() = 0;

  
  
  virtual void ReplacedWithAnotherClient() = 0;

  
  
  static DevToolsClientHost* CreateDevToolsFrontendHost(
      WebContents* client_web_contents,
      DevToolsFrontendHostDelegate* delegate);

  
  static void SetupDevToolsFrontendClient(RenderViewHost* frontend_rvh);
};

}  

#endif  
