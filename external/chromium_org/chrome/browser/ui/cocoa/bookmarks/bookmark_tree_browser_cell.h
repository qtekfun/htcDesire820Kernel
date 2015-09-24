// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_TREE_BROWSER_CELL_H_
#define CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_TREE_BROWSER_CELL_H_

#import <Cocoa/Cocoa.h>

class BookmarkNode;

@interface BookmarkTreeBrowserCell : NSBrowserCell {
 @private
  const BookmarkNode* bookmarkNode_;  
  NSMatrix* matrix_;  
  id target_;  
  SEL action_;
}

@property(nonatomic, assign) NSMatrix* matrix;
@property(nonatomic, assign) id target;
@property(nonatomic, assign) SEL action;

- (const BookmarkNode*)bookmarkNode;
- (void)setBookmarkNode:(const BookmarkNode*)bookmarkNode;

@end

#endif  
