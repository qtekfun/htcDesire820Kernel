// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_DEFAULTS_H_
#define CHROME_BROWSER_DEFAULTS_H_

#include "build/build_config.h"
#include "chrome/browser/prefs/session_startup_pref.h"
#include "ui/base/resource/resource_bundle.h"

namespace browser_defaults {

#if defined(USE_X11)

extern const bool kCanToggleSystemTitleBar;

#endif

extern const int kOmniboxFontPixelSize;

#if defined(TOOLKIT_VIEWS)
const int kMiniTabWidth = 64;
#else
const int kMiniTabWidth = 56;
#endif

extern const bool kRestorePopups;

extern const bool kBrowserAliveWithNoWindows;

extern const bool kShowExitMenuItem;
extern const bool kShowHelpMenuItemIcon;
extern const bool kShowUpgradeMenuItem;

extern const bool kShowImportOnBookmarkBar;

extern const bool kAlwaysOpenIncognitoWindow;

extern const bool kAlwaysCreateTabbedBrowserOnSessionRestore;

extern const bool kDownloadPageHasShowInFolder;

extern const bool kSizeTabButtonToTopOfTabStrip;

extern const bool kSyncAutoStarts;

extern const bool kShowOtherBrowsersInAboutMemory;

extern const bool kShowCancelButtonInTaskManager;

extern const bool kScrollEventChangesTab;

extern const ui::ResourceBundle::FontStyle kAssociatedNetworkFontStyle;

extern const bool kPasswordEchoEnabled;


extern bool bookmarks_enabled;

extern bool enable_help_app;

}  

#endif  
