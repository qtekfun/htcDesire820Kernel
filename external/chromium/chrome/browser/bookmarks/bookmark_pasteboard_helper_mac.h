// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BOOKMARKS_BOOKMARK_PASTEBOARD_HELPER_MAC_H_
#define CHROME_BROWSER_BOOKMARKS_BOOKMARK_PASTEBOARD_HELPER_MAC_H_
#pragma once

#include "base/file_path.h"
#include "chrome/browser/bookmarks/bookmark_node_data.h"
#include "ui/gfx/native_widget_types.h"

namespace bookmark_pasteboard_helper_mac {

void WriteToClipboard(const std::vector<BookmarkNodeData::Element>& elements,
                      FilePath::StringType profile_path);

void WriteToDragClipboard(
    const std::vector<BookmarkNodeData::Element>& elements,
    FilePath::StringType profile_path);

bool ReadFromClipboard(std::vector<BookmarkNodeData::Element>& elements,
                       FilePath::StringType* profile_path);

bool ReadFromDragClipboard(std::vector<BookmarkNodeData::Element>& elements,
                           FilePath::StringType* profile_path);

bool ClipboardContainsBookmarks();

bool DragClipboardContainsBookmarks();

void StartDrag(Profile* profile, const std::vector<const BookmarkNode*>& nodes,
               gfx::NativeView view);

}

#ifdef __OBJC__
@class NSString;
extern "C" NSString* const kBookmarkDictionaryListPboardType;
#endif  

#endif  
