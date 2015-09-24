// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_MESSAGE_HANDLER_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_MESSAGE_HANDLER_H_
#pragma once

#include "content/browser/renderer_host/render_view_host_observer.h"

class Profile;
struct ExtensionHostMsg_DomMessage_Params;

class ExtensionMessageHandler : public RenderViewHostObserver {
 public:
  
  explicit ExtensionMessageHandler(RenderViewHost* render_view_host);
  virtual ~ExtensionMessageHandler();

  
  virtual bool OnMessageReceived(const IPC::Message& message);

 private:
  
  void OnPostMessage(int port_id, const std::string& message);
  void OnRequest(const ExtensionHostMsg_DomMessage_Params& params);

  DISALLOW_COPY_AND_ASSIGN(ExtensionMessageHandler);
};

#endif  
