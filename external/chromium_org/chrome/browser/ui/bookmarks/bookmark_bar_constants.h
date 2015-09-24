// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_BOOKMARKS_BOOKMARK_BAR_CONSTANTS_H_
#define CHROME_BROWSER_UI_BOOKMARKS_BOOKMARK_BAR_CONSTANTS_H_

#include "build/build_config.h"

namespace chrome {

#if defined(TOOLKIT_GTK)
const int kNTPBookmarkBarHeight = 57;
#elif defined(TOOLKIT_VIEWS) || defined(OS_MACOSX)
const int kNTPBookmarkBarHeight = 40;
#endif

#if defined(TOOLKIT_VIEWS)
const int kBookmarkBarHeight = 28;
#elif defined(OS_MACOSX)
const int kBookmarkBarHeight = 26;
#elif defined(TOOLKIT_GTK)
const int kBookmarkBarHeight = 29;
#endif

}  

#endif  
