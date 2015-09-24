// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_BOOKMARKS_BOOKMARK_DRAG_DROP_VIEWS_H_
#define CHROME_BROWSER_UI_VIEWS_BOOKMARKS_BOOKMARK_DRAG_DROP_VIEWS_H_

class BookmarkNode;
struct BookmarkNodeData;
class Profile;

namespace content {
class BrowserContext;
}

namespace ui {
class DropTargetEvent;
}

namespace chrome {

int GetBookmarkDragOperation(content::BrowserContext* browser_context,
                             const BookmarkNode* node);

int GetPreferredBookmarkDropOperation(int source_operations, int operations);

int GetBookmarkDropOperation(Profile* profile,
                             const ui::DropTargetEvent& event,
                             const BookmarkNodeData& data,
                             const BookmarkNode* parent,
                             int index);

bool IsValidBookmarkDropLocation(Profile* profile,
                                 const BookmarkNodeData& data,
                                 const BookmarkNode* drop_parent,
                                 int index);

}  

#endif  
