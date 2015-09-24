// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_TERMINAL_TERMINAL_PRIVATE_API_H_
#define CHROME_BROWSER_EXTENSIONS_API_TERMINAL_TERMINAL_PRIVATE_API_H_

#include <string>

#include "chrome/browser/extensions/chrome_extension_function.h"

namespace extensions {
class TerminalPrivateFunction : public ChromeAsyncExtensionFunction {
 public:
  TerminalPrivateFunction();

 protected:
  virtual ~TerminalPrivateFunction();

  
  virtual bool RunImpl() OVERRIDE;

  
  virtual bool RunTerminalFunction() = 0;
};

class TerminalPrivateOpenTerminalProcessFunction
    : public TerminalPrivateFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("terminalPrivate.openTerminalProcess",
                             TERMINALPRIVATE_OPENTERMINALPROCESS)

  TerminalPrivateOpenTerminalProcessFunction();

 protected:
  virtual ~TerminalPrivateOpenTerminalProcessFunction();

  
  virtual bool RunTerminalFunction() OVERRIDE;

 private:
  void OpenOnFileThread();
  void RespondOnUIThread(pid_t pid);

  const char* command_;
};

class TerminalPrivateSendInputFunction : public TerminalPrivateFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("terminalPrivate.sendInput",
                             TERMINALPRIVATE_SENDINPUT)

 protected:
  virtual ~TerminalPrivateSendInputFunction();

  
  virtual bool RunTerminalFunction() OVERRIDE;

 private:
  void SendInputOnFileThread(pid_t pid, const std::string& input);
  void RespondOnUIThread(bool success);
};

class TerminalPrivateCloseTerminalProcessFunction
    : public TerminalPrivateFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("terminalPrivate.closeTerminalProcess",
                             TERMINALPRIVATE_CLOSETERMINALPROCESS)

 protected:
  virtual ~TerminalPrivateCloseTerminalProcessFunction();

  virtual bool RunTerminalFunction() OVERRIDE;

 private:
  void CloseOnFileThread(pid_t pid);
  void RespondOnUIThread(bool success);
};

class TerminalPrivateOnTerminalResizeFunction : public TerminalPrivateFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("terminalPrivate.onTerminalResize",
                             TERMINALPRIVATE_ONTERMINALRESIZE)

 protected:
  virtual ~TerminalPrivateOnTerminalResizeFunction();

  virtual bool RunTerminalFunction() OVERRIDE;

 private:
  void OnResizeOnFileThread(pid_t pid, int width, int height);
  void RespondOnUIThread(bool success);
};

}  

#endif  
