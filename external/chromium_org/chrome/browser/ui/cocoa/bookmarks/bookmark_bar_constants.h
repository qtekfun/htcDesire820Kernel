// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_BAR_CONSTANTS_H_
#define CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_BAR_CONSTANTS_H_

#include "chrome/browser/ui/bookmarks/bookmark_bar_constants.h"

namespace bookmarks {

const int kVisualHeightOffset = 2;

const int kNTPBookmarkBarPadding =
    (chrome::kNTPBookmarkBarHeight -
        (chrome::kBookmarkBarHeight + kVisualHeightOffset)) / 2;

const int kBookmarkButtonHeight =
    chrome::kBookmarkBarHeight + kVisualHeightOffset;

const CGFloat kBookmarkFolderButtonHeight = 24.0;

const CGFloat kBookmarkBarMenuCornerRadius = 4.0;

const CGFloat kBookmarkBarOverlap = 3.0;

}  

#endif  
