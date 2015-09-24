// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_AUTOMATION_AUTOMATION_PROVIDER_JSON_H_
#define CHROME_BROWSER_AUTOMATION_AUTOMATION_PROVIDER_JSON_H_

#include <string>

#include "base/compiler_specific.h"
#include "chrome/common/automation_constants.h"

class AutomationProvider;
class Browser;
class Profile;

namespace base {
class DictionaryValue;
class Value;
}

namespace content {
class RenderViewHost;
class WebContents;
}

namespace extensions {
class Extension;
}

namespace IPC {
class Message;
}

class AutomationJSONReply {
 public:
  
  
  
  AutomationJSONReply(AutomationProvider* provider,
                      IPC::Message* reply_message);

  ~AutomationJSONReply();

  
  
  void SendSuccess(const base::Value* value);

  
  void SendError(const std::string& error_message);

 private:
  AutomationProvider* provider_;
  IPC::Message* message_;
};

bool GetBrowserFromJSONArgs(base::DictionaryValue* args,
                            Browser** browser,
                            std::string* error) WARN_UNUSED_RESULT;

bool GetTabFromJSONArgs(base::DictionaryValue* args,
                        content::WebContents** tab,
                        std::string* error) WARN_UNUSED_RESULT;

bool GetBrowserAndTabFromJSONArgs(base::DictionaryValue* args,
                                  Browser** browser,
                                  content::WebContents** tab,
                                  std::string* error) WARN_UNUSED_RESULT;

bool GetRenderViewFromJSONArgs(
    base::DictionaryValue* args,
    Profile* profile,
    content::RenderViewHost** rvh,
    std::string* error) WARN_UNUSED_RESULT;

bool GetExtensionFromJSONArgs(
    base::DictionaryValue* args,
    const std::string& key,
    Profile* profile,
    const extensions::Extension** extension,
    std::string* error) WARN_UNUSED_RESULT;

bool GetEnabledExtensionFromJSONArgs(
    base::DictionaryValue* args,
    const std::string& key,
    Profile* profile,
    const extensions::Extension** extension,
    std::string* error) WARN_UNUSED_RESULT;

#endif  
