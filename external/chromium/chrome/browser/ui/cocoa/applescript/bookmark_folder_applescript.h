// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_APPLESCRIPT_BOOKMARK_FOLDER_APPLESCRIPT_H_
#define CHROME_BROWSER_UI_COCOA_APPLESCRIPT_BOOKMARK_FOLDER_APPLESCRIPT_H_

#import <Cocoa/Cocoa.h>

#import "chrome/browser/ui/cocoa/applescript/bookmark_node_applescript.h"

@class BookmarkItemAppleScript;

@interface BookmarkFolderAppleScript : BookmarkNodeAppleScript {

}

- (NSArray*)bookmarkFolders;

- (void)insertInBookmarkFolders:(id)aBookmarkFolder;

- (void)insertInBookmarkFolders:(id)aBookmarkFolder atIndex:(int)index;

- (void)removeFromBookmarkFoldersAtIndex:(int)index;

- (NSArray*)bookmarkItems;

- (void)insertInBookmarkItems:(BookmarkItemAppleScript*)aBookmarkItem;

- (void)insertInBookmarkItems:(BookmarkItemAppleScript*)aBookmarkItem
                      atIndex:(int)index;

- (void)removeFromBookmarkItemsAtIndex:(int)index;

- (int)calculatePositionOfBookmarkFolderAt:(int)index;

- (int)calculatePositionOfBookmarkItemAt:(int)index;

@end

#endif  
