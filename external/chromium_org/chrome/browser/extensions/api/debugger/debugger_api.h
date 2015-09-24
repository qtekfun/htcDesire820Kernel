// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_EXTENSIONS_API_DEBUGGER_DEBUGGER_API_H_
#define CHROME_BROWSER_EXTENSIONS_API_DEBUGGER_DEBUGGER_API_H_

#include <string>
#include <vector>

#include "chrome/browser/extensions/chrome_extension_function.h"
#include "chrome/common/extensions/api/debugger.h"

using extensions::api::debugger::Debuggee;


class ExtensionDevToolsClientHost;
class DevToolsTargetImpl;

namespace base {
class DictionaryValue;
}

namespace content {
class DevToolsAgentHost;
class WebContents;
}

class DebuggerFunction : public ChromeAsyncExtensionFunction {
 protected:
  DebuggerFunction();
  virtual ~DebuggerFunction();

  void FormatErrorMessage(const std::string& format);

  bool InitAgentHost();
  bool InitClientHost();

  Debuggee debuggee_;
  scoped_refptr<content::DevToolsAgentHost> agent_host_;
  ExtensionDevToolsClientHost* client_host_;
};

class DebuggerAttachFunction : public DebuggerFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("debugger.attach", DEBUGGER_ATTACH)

  DebuggerAttachFunction();

 protected:
  virtual ~DebuggerAttachFunction();

  
  virtual bool RunImpl() OVERRIDE;
};

class DebuggerDetachFunction : public DebuggerFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("debugger.detach", DEBUGGER_DETACH)

  DebuggerDetachFunction();

 protected:
  virtual ~DebuggerDetachFunction();

  
  virtual bool RunImpl() OVERRIDE;
};

class DebuggerSendCommandFunction : public DebuggerFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("debugger.sendCommand", DEBUGGER_SENDCOMMAND)

  DebuggerSendCommandFunction();
  void SendResponseBody(base::DictionaryValue* result);

 protected:
  virtual ~DebuggerSendCommandFunction();

  
  virtual bool RunImpl() OVERRIDE;
};

class DebuggerGetTargetsFunction : public DebuggerFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("debugger.getTargets", DEBUGGER_ATTACH)

  DebuggerGetTargetsFunction();

 protected:
  virtual ~DebuggerGetTargetsFunction();

  
  virtual bool RunImpl() OVERRIDE;

 private:
  void SendTargetList(const std::vector<DevToolsTargetImpl*>& target_list);
};

#endif  
