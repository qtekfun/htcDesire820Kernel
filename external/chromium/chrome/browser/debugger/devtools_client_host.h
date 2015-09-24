// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DEBUGGER_DEVTOOLS_CLIENT_HOST_H_
#define CHROME_BROWSER_DEBUGGER_DEVTOOLS_CLIENT_HOST_H_
#pragma once

#include <string>

#include "base/basictypes.h"

namespace IPC {
class Message;
}

class DevToolsWindow;
class TabContentsWrapper;

class DevToolsClientHost {
 public:
  class CloseListener {
   public:
    CloseListener() {}
    virtual ~CloseListener() {}
    virtual void ClientHostClosing(DevToolsClientHost* host) = 0;
   private:
    DISALLOW_COPY_AND_ASSIGN(CloseListener);
  };

  virtual ~DevToolsClientHost() {}

  
  
  virtual void InspectedTabClosing() = 0;

  
  
  virtual void FrameNavigating(const std::string& url) = 0;

  
  virtual void SendMessageToClient(const IPC::Message& msg) = 0;

  void set_close_listener(CloseListener* listener) {
    close_listener_ = listener;
  }

  virtual DevToolsWindow* AsDevToolsWindow();

  
  
  virtual void TabReplaced(TabContentsWrapper* new_tab) = 0;

 protected:
  DevToolsClientHost() : close_listener_(NULL) {}

  
  
  void NotifyCloseListener();

 private:
  CloseListener* close_listener_;
  DISALLOW_COPY_AND_ASSIGN(DevToolsClientHost);
};

#endif  
