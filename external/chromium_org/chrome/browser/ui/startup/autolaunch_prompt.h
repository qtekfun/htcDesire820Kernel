// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_STARTUP_AUTOLAUNCH_PROMPT_H_
#define CHROME_BROWSER_UI_STARTUP_AUTOLAUNCH_PROMPT_H_

class Browser;

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace chrome {

bool ShowAutolaunchPrompt(Browser* browser);

void RegisterAutolaunchUserPrefs(user_prefs::PrefRegistrySyncable* registry);

}  

#endif  
