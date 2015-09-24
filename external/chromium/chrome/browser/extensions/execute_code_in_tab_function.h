// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXECUTE_CODE_IN_TAB_FUNCTION_H__
#define CHROME_BROWSER_EXTENSIONS_EXECUTE_CODE_IN_TAB_FUNCTION_H__
#pragma once

#include <string>

#include "chrome/browser/extensions/extension_function.h"
#include "chrome/common/extensions/extension_resource.h"
#include "content/browser/tab_contents/tab_contents_observer.h"

class ExecuteCodeInTabFunction : public AsyncExtensionFunction,
                                 public TabContentsObserver {
 public:
  ExecuteCodeInTabFunction();
  virtual ~ExecuteCodeInTabFunction();

 private:
  virtual bool RunImpl();

  
  virtual bool OnMessageReceived(const IPC::Message& message);

  
  void OnExecuteCodeFinished(int request_id, bool success,
                             const std::string& error);

  
  
  void DidLoadFile(bool success, const std::string& data);

  
  
  bool Execute(const std::string& code_string);

  TabContentsObserver::Registrar registrar_;

  
  int execute_tab_id_;

  
  
  ExtensionResource resource_;

  
  
  bool all_frames_;
};

class TabsExecuteScriptFunction : public ExecuteCodeInTabFunction {
  DECLARE_EXTENSION_FUNCTION_NAME("tabs.executeScript")
};

class TabsInsertCSSFunction : public ExecuteCodeInTabFunction {
  DECLARE_EXTENSION_FUNCTION_NAME("tabs.insertCSS")
};

#endif  
