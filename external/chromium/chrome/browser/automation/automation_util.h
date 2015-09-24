// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOMATION_AUTOMATION_UTIL_H_
#define CHROME_BROWSER_AUTOMATION_AUTOMATION_UTIL_H_
#pragma once

#include <string>

#include "base/basictypes.h"

class AutomationProvider;
class Browser;
class DictionaryValue;
class GURL;
class TabContents;

namespace IPC {
class Message;
}


namespace automation_util {

Browser* GetBrowserAt(int index);

TabContents* GetTabContentsAt(int browser_index, int tab_index);

void GetCookies(const GURL& url,
                TabContents* contents,
                int* value_size,
                std::string* value);

void SetCookie(const GURL& url,
               const std::string& value,
               TabContents* contents,
               int* response_value);

void DeleteCookie(const GURL& url,
                  const std::string& cookie_name,
                  TabContents* contents,
                  bool* success);

void GetCookiesJSON(AutomationProvider* provider,
                    DictionaryValue* args,
                    IPC::Message* reply_message);

void DeleteCookieJSON(AutomationProvider* provider,
                      DictionaryValue* args,
                      IPC::Message* reply_message);

void SetCookieJSON(AutomationProvider* provider,
                   DictionaryValue* args,
                   IPC::Message* reply_message);

}  

#endif  
