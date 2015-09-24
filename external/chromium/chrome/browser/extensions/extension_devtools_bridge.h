// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_DEVTOOLS_BRIDGE_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_DEVTOOLS_BRIDGE_H_
#pragma once

#include <string>

#include "base/memory/ref_counted.h"
#include "chrome/browser/debugger/devtools_client_host.h"
#include "chrome/browser/extensions/extension_devtools_manager.h"
#include "chrome/browser/extensions/extension_message_service.h"

class Profile;

class ExtensionDevToolsBridge : public DevToolsClientHost {
 public:
  ExtensionDevToolsBridge(int tab_id, Profile* profile);
  virtual ~ExtensionDevToolsBridge();

  bool RegisterAsDevToolsClientHost();
  void UnregisterAsDevToolsClientHost();

  
  
  virtual void InspectedTabClosing();

  
  virtual void SendMessageToClient(const IPC::Message& msg);

  virtual void TabReplaced(TabContentsWrapper* new_tab);

 private:
  void OnDispatchOnInspectorFrontend(const std::string& data);

  virtual void FrameNavigating(const std::string& url) {}

  
  int tab_id_;

  scoped_refptr<ExtensionDevToolsManager> extension_devtools_manager_;
  scoped_refptr<ExtensionMessageService> extension_message_service_;

  
  Profile* profile_;

  
  
  const std::string on_page_event_name_;
  const std::string on_tab_close_event_name_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionDevToolsBridge);
};

#endif  

