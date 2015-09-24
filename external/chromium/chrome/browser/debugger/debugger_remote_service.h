// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DEBUGGER_DEBUGGER_REMOTE_SERVICE_H_
#define CHROME_BROWSER_DEBUGGER_DEBUGGER_REMOTE_SERVICE_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "chrome/browser/debugger/devtools_remote.h"

class DevToolsProtocolHandler;
class DevToolsRemoteMessage;
class DictionaryValue;
class Value;
class TabContents;

struct DebuggerRemoteServiceCommand {
  static const std::string kAttach;
  static const std::string kDetach;
  static const std::string kDebuggerCommand;
  static const std::string kEvaluateJavascript;
  static const std::string kFrameNavigate;  
  static const std::string kTabClosed;  
};

class DebuggerRemoteService : public DevToolsRemoteListener {
 public:
  
  
  
  
  explicit DebuggerRemoteService(DevToolsProtocolHandler* delegate);

  
  void DebuggerOutput(int32 tab_uid, const std::string& message);

  
  void FrameNavigate(int32 tab_uid, const std::string& url);

  
  void TabClosed(int32 tab_uid);

  
  
  
  // If |response| is not NULL, the operation result will be written
  
  
  void DetachFromTab(const std::string& destination,
                     DictionaryValue* response);

  

  
  
  
  virtual void HandleMessage(const DevToolsRemoteMessage& message);

  
  
  virtual void OnConnectionLost();

  
  static const std::string kToolName;

 private:
  
  typedef enum {
    RESULT_OK = 0,
    RESULT_ILLEGAL_TAB_STATE,
    RESULT_UNKNOWN_TAB,
    RESULT_DEBUGGER_ERROR,
    RESULT_UNKNOWN_COMMAND
  } Result;

  virtual ~DebuggerRemoteService();

  
  
  void AttachToTab(const std::string& destination,
                   DictionaryValue* response);

  
  
  
  TabContents* ToTabContents(int32 tab_uid);

  
  
  void SendResponse(const Value& response,
                    const std::string& tool,
                    const std::string& destination);

  
  
  
  bool DispatchDebuggerCommand(int tab_uid,
                               DictionaryValue* content,
                               DictionaryValue* response);

  
  
  
  bool DispatchEvaluateJavascript(int tab_uid,
                                  DictionaryValue* content,
                                  DictionaryValue* response);

  
  DevToolsProtocolHandler* delegate_;
  DISALLOW_COPY_AND_ASSIGN(DebuggerRemoteService);
};

#endif  
