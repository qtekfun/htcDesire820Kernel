// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_BAR_CONSTANTS_H_
#define CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_BAR_CONSTANTS_H_
#pragma once

namespace bookmarks {

const int kVisualHeightOffset = 2;

const int kBookmarkBarHeight = 26;

const int kNTPBookmarkBarHeight = 40;

const int kNTPBookmarkBarPadding =
    (kNTPBookmarkBarHeight - (kBookmarkBarHeight + kVisualHeightOffset)) / 2;

const int kBookmarkButtonHeight = kBookmarkBarHeight + kVisualHeightOffset;

const CGFloat kBookmarkFolderButtonHeight = 24.0;

const CGFloat kBookmarkBarMenuCornerRadius = 4.0;

}  

#endif  
