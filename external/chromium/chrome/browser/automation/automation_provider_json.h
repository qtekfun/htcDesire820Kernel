// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_AUTOMATION_AUTOMATION_PROVIDER_JSON_H_
#define CHROME_BROWSER_AUTOMATION_AUTOMATION_PROVIDER_JSON_H_
#pragma once

#include <string>

#include "base/compiler_specific.h"

class AutomationProvider;
class Browser;
class DictionaryValue;
class TabContents;
class Value;

namespace IPC {
class Message;
}

class AutomationJSONReply {
 public:
  
  
  
  AutomationJSONReply(AutomationProvider* provider,
                      IPC::Message* reply_message);

  ~AutomationJSONReply();

  
  
  void SendSuccess(const Value* value);

  
  void SendError(const std::string& error_message);

 private:
  AutomationProvider* provider_;
  IPC::Message* message_;
};

bool GetBrowserFromJSONArgs(DictionaryValue* args,
                            Browser** browser,
                            std::string* error) WARN_UNUSED_RESULT;

bool GetTabFromJSONArgs(DictionaryValue* args,
                        TabContents** tab,
                        std::string* error) WARN_UNUSED_RESULT;

bool GetBrowserAndTabFromJSONArgs(DictionaryValue* args,
                                  Browser** browser,
                                  TabContents** tab,
                                  std::string* error) WARN_UNUSED_RESULT;

#endif  
