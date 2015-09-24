// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_STARTUP_DEFAULT_BROWSER_PROMPT_H_
#define CHROME_BROWSER_UI_STARTUP_DEFAULT_BROWSER_PROMPT_H_

#include "chrome/browser/ui/host_desktop.h"

class PrefRegistrySimple;
class Profile;

namespace chrome {

void RegisterDefaultBrowserPromptPrefs(PrefRegistrySimple* registry);

void ShowDefaultBrowserPrompt(Profile* profile, HostDesktopType desktop_type);

bool ShowFirstRunDefaultBrowserPrompt(Profile* profile);

}  

#endif  
