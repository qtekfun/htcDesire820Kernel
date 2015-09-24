// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BOOKMARKS_BOOKMARK_PASTEBOARD_HELPER_MAC_H_
#define CHROME_BROWSER_BOOKMARKS_BOOKMARK_PASTEBOARD_HELPER_MAC_H_

#include "chrome/browser/bookmarks/bookmark_node_data.h"

#if defined(__OBJC__)
@class NSString;
#endif  

namespace base {
class FilePath;
}


void WriteBookmarksToPasteboard(
    ui::ClipboardType type,
    const std::vector<BookmarkNodeData::Element>& elements,
    const base::FilePath& profile_path);

bool ReadBookmarksFromPasteboard(
    ui::ClipboardType type,
    std::vector<BookmarkNodeData::Element>& elements,
    base::FilePath* profile_path);

bool PasteboardContainsBookmarks(ui::ClipboardType type);

#if defined(__OBJC__)
extern "C" NSString* const kBookmarkDictionaryListPboardType;
#endif  

#endif  
