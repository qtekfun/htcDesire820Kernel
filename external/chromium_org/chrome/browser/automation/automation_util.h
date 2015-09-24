// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOMATION_AUTOMATION_UTIL_H_
#define CHROME_BROWSER_AUTOMATION_AUTOMATION_UTIL_H_

#include <string>

#include "base/basictypes.h"

class AutomationProvider;
class Browser;
class GURL;
class Profile;

namespace content {
class RenderViewHost;
class WebContents;
}

namespace base {
class DictionaryValue;
}

namespace extensions {
class Extension;
}

namespace IPC {
class Message;
}


namespace automation_util {

Browser* GetBrowserAt(int index);

content::WebContents* GetWebContentsAt(int browser_index, int tab_index);

#if defined(OS_CHROMEOS)
Profile* GetCurrentProfileOnChromeOS(std::string* error_message);
#endif

Browser* GetBrowserForTab(content::WebContents* tab);

void GetCookies(const GURL& url,
                content::WebContents* contents,
                int* value_size,
                std::string* value);

void SetCookie(const GURL& url,
               const std::string& value,
               content::WebContents* contents,
               int* response_value);

void DeleteCookie(const GURL& url,
                  const std::string& cookie_name,
                  content::WebContents* contents,
                  bool* success);

void GetCookiesJSON(AutomationProvider* provider,
                    base::DictionaryValue* args,
                    IPC::Message* reply_message);

void DeleteCookieJSON(AutomationProvider* provider,
                      base::DictionaryValue* args,
                      IPC::Message* reply_message);

void SetCookieJSON(AutomationProvider* provider,
                   base::DictionaryValue* args,
                   IPC::Message* reply_message);

bool SendErrorIfModalDialogActive(AutomationProvider* provider,
                                  IPC::Message* message);

}  

#endif  
