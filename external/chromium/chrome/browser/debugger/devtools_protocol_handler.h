// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DEBUGGER_DEVTOOLS_PROTOCOL_HANDLER_H_
#define CHROME_BROWSER_DEBUGGER_DEVTOOLS_PROTOCOL_HANDLER_H_
#pragma once

#include <string>

#include "base/hash_tables.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/debugger/devtools_remote.h"
#include "net/base/listen_socket.h"

class InspectableTabProxy;
class DevToolsRemoteListenSocket;
class DevToolsRemoteMessage;

class DevToolsProtocolHandler
    : public DevToolsRemoteListener,
      public OutboundSocketDelegate {
 public:
  typedef base::hash_map< std::string, scoped_refptr<DevToolsRemoteListener> >
      ToolToListenerMap;

  static scoped_refptr<DevToolsProtocolHandler> Start(int port);

  
  
  void Stop();

  
  
  
  
  
  void RegisterDestination(DevToolsRemoteListener* listener,
                           const std::string& tool_name);

  
  
  void UnregisterDestination(DevToolsRemoteListener* listener,
                             const std::string& tool_name);

  InspectableTabProxy* inspectable_tab_proxy() {
    return inspectable_tab_proxy_.get();
  }

  
  virtual void HandleMessage(const DevToolsRemoteMessage& message);
  virtual void OnAcceptConnection(ListenSocket *connection);
  virtual void OnConnectionLost();

  
  virtual void Send(const DevToolsRemoteMessage& message);

 private:
  explicit DevToolsProtocolHandler(int port);
  virtual ~DevToolsProtocolHandler();
  void Start();

  void Init();
  void Teardown();
  int port_;
  ToolToListenerMap tool_to_listener_map_;
  scoped_refptr<ListenSocket> connection_;
  scoped_refptr<DevToolsRemoteListenSocket> server_;
  scoped_ptr<InspectableTabProxy> inspectable_tab_proxy_;
  DISALLOW_COPY_AND_ASSIGN(DevToolsProtocolHandler);
};

#endif  
