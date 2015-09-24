// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEB_APPLICATIONS_WEB_APP_UI_H_
#define CHROME_BROWSER_UI_WEB_APPLICATIONS_WEB_APP_UI_H_
#pragma once

#include <vector>

#include "chrome/browser/shell_integration.h"

class TabContentsWrapper;

namespace web_app {

void GetShortcutInfoForTab(TabContentsWrapper* tab_contents,
                           ShellIntegration::ShortcutInfo* info);

void UpdateShortcutForTabContents(TabContentsWrapper* tab_contents);

}  

#endif  
