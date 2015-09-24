// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_DEBUGGER_EXTENSION_PORTS_REMOTE_SERVICE_H_
#define CHROME_BROWSER_DEBUGGER_EXTENSION_PORTS_REMOTE_SERVICE_H_
#pragma once

#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/debugger/devtools_remote.h"
#include "chrome/browser/extensions/extension_message_service.h"
#include "ipc/ipc_message.h"

class DevToolsProtocolHandler;
class DevToolsRemoteMessage;
class DictionaryValue;
class GURL;
class ListValue;
class Value;

class ExtensionPortsRemoteService : public DevToolsRemoteListener,
                                    public IPC::Message::Sender {
 public:
  
  static const std::string kToolName;

  
  
  
  explicit ExtensionPortsRemoteService(DevToolsProtocolHandler* delegate);

  

  
  
  virtual void HandleMessage(const DevToolsRemoteMessage& message);

  
  
  virtual void OnConnectionLost();

  

  
  
  virtual bool Send(IPC::Message* msg);

 private:
  
  
  typedef enum {
    RESULT_OK = 0,
    RESULT_UNKNOWN_COMMAND,
    RESULT_NO_SERVICE,
    RESULT_PARAMETER_ERROR,
    RESULT_UNKNOWN_PORT,
    RESULT_TAB_NOT_FOUND,
    RESULT_CONNECT_FAILED,  
  } Result;

  virtual ~ExtensionPortsRemoteService();

  
  
  void SendResponse(const Value& response,
                    const std::string& tool,
                    const std::string& destination);

  
  void OnExtensionMessageInvoke(const std::string& extension_id,
                                const std::string& function_name,
                                const ListValue& args,
                                const GURL& event_url);
  
  
  void OnExtensionMessage(const std::string& message, int port_id);
  
  void OnExtensionPortDisconnected(int port_id);

  
  
  void ConnectCommand(DictionaryValue* content, DictionaryValue* response);
  
  void DisconnectCommand(int port_id, DictionaryValue* response);
  
  void PostMessageCommand(int port_id, DictionaryValue* content,
                          DictionaryValue* response);

  
  
  DevToolsProtocolHandler* delegate_;

  
  typedef std::set<int> PortIdSet;
  PortIdSet openPortIds_;

  scoped_refptr<ExtensionMessageService> service_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionPortsRemoteService);
};

#endif  
