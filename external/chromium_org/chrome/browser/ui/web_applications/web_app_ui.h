// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEB_APPLICATIONS_WEB_APP_UI_H_
#define CHROME_BROWSER_UI_WEB_APPLICATIONS_WEB_APP_UI_H_

#include <vector>

#include "base/callback.h"
#include "chrome/browser/shell_integration.h"

namespace content {
class WebContents;
}

namespace extensions {
class Extension;
}

class Profile;

namespace web_app {

typedef base::Callback<void(const ShellIntegration::ShortcutInfo&)>
    ShortcutInfoCallback;

ShellIntegration::ShortcutInfo ShortcutInfoForExtensionAndProfile(
    const extensions::Extension* extension, Profile* profile);

void GetShortcutInfoForTab(content::WebContents* web_contents,
                           ShellIntegration::ShortcutInfo* info);

void UpdateShortcutForTabContents(content::WebContents* web_contents);

void UpdateShortcutInfoForApp(const extensions::Extension& extension,
                              Profile* profile,
                              ShellIntegration::ShortcutInfo* shortcut_info);

void UpdateShortcutInfoAndIconForApp(
    const extensions::Extension& extension,
    Profile* profile,
    const ShortcutInfoCallback& callback);

}  

#endif  
