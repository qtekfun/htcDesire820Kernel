// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_DEVTOOLS_DEVTOOLS_FRONTEND_HOST_H_
#define CONTENT_BROWSER_DEVTOOLS_DEVTOOLS_FRONTEND_HOST_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "content/public/browser/devtools_client_host.h"
#include "content/public/browser/web_contents_observer.h"

namespace content {

class DevToolsFrontendHostDelegate;
class WebContentsImpl;

class DevToolsFrontendHost : public DevToolsClientHost,
                             public WebContentsObserver {
 public:
  DevToolsFrontendHost(WebContentsImpl* web_contents,
                       DevToolsFrontendHostDelegate* delegate);

 private:
  virtual ~DevToolsFrontendHost();

  
  virtual void DispatchOnInspectorFrontend(const std::string& message) OVERRIDE;
  virtual void InspectedContentsClosing() OVERRIDE;
  virtual void ReplacedWithAnotherClient() OVERRIDE;

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void RenderProcessGone(base::TerminationStatus status) OVERRIDE;

  void OnDispatchOnInspectorBackend(const std::string& message);
  void OnDispatchOnEmbedder(const std::string& message);

  DevToolsFrontendHostDelegate* delegate_;
  DISALLOW_COPY_AND_ASSIGN(DevToolsFrontendHost);
};

}  

#endif  
