// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_DEFAULTS_H_
#define CHROME_BROWSER_DEFAULTS_H_
#pragma once

#include "build/build_config.h"
#include "chrome/browser/prefs/session_startup_pref.h"

namespace browser_defaults {

#if defined(USE_X11)

extern const int kAutocompleteEditFontPixelSize;

extern const int kAutocompleteEditFontPixelSizeInPopup;

extern const bool kCanToggleSystemTitleBar;

#endif

extern const SessionStartupPref::Type kDefaultSessionStartupType;

extern const int kMiniTabWidth;

extern const bool kRestorePopups;

extern const bool kBrowserAliveWithNoWindows;

extern const bool kShowImportOnBookmarkBar;

extern const bool kShowExitMenuItem;

extern const bool kOSSupportsOtherBrowsers;

extern const bool kDownloadPageHasShowInFolder;

extern const bool kSizeTabButtonToTopOfTabStrip;

extern const bool kBootstrapSyncAuthentication;

extern const bool kShowOtherBrowsersInAboutMemory;

extern const bool kAlwaysOpenIncognitoWindow;

extern const bool kShowCancelButtonInTaskManager;

extern const bool kEnableTouchIcon;


extern bool bookmarks_enabled;

extern bool skip_restore;

extern bool enable_help_app;

}  

#endif  
