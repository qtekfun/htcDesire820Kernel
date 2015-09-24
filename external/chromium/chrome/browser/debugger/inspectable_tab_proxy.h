// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DEBUGGER_INSPECTABLE_TAB_PROXY_H_
#define CHROME_BROWSER_DEBUGGER_INSPECTABLE_TAB_PROXY_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/hash_tables.h"
#include "chrome/browser/debugger/devtools_client_host.h"

class DebuggerRemoteService;
class DevToolsClientHost;
class DevToolsClientHostImpl;
class NavigationController;
struct DevToolsMessageData;

class InspectableTabProxy {
 public:
  typedef base::hash_map<int32, NavigationController*> ControllersMap;
  typedef base::hash_map<int32, DevToolsClientHostImpl*> IdToClientHostMap;

  InspectableTabProxy();
  virtual ~InspectableTabProxy();

  
  
  
  
  const ControllersMap& controllers_map();

  
  DevToolsClientHostImpl* ClientHostForTabId(int32 id);

  
  
  
  
  DevToolsClientHost* NewClientHost(int32 id,
                                    DebuggerRemoteService* service);

  
  
  
  void OnRemoteDebuggerDetached();

 private:
  ControllersMap controllers_map_;
  IdToClientHostMap id_to_client_host_map_;
  DISALLOW_COPY_AND_ASSIGN(InspectableTabProxy);
};


class DevToolsClientHostImpl : public DevToolsClientHost {
 public:
  DevToolsClientHostImpl(
    int32 id,
    DebuggerRemoteService* service,
    InspectableTabProxy::IdToClientHostMap* map);
  ~DevToolsClientHostImpl();

  DebuggerRemoteService* debugger_remote_service() {
    return service_;
  }

  void Close();

  
  virtual void InspectedTabClosing();
  virtual void SendMessageToClient(const IPC::Message& msg);
  virtual void TabReplaced(TabContentsWrapper* new_tab);

 private:
  
  void OnDebuggerOutput(const std::string& msg);
  virtual void FrameNavigating(const std::string& url);
  void TabClosed();

  int32 id_;
  DebuggerRemoteService* service_;
  InspectableTabProxy::IdToClientHostMap* map_;
};

#endif  
