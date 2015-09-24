// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_APPLESCRIPT_BROWSERCRAPPLICATION_APPLESCRIPT_H_
#define CHROME_BROWSER_UI_COCOA_APPLESCRIPT_BROWSERCRAPPLICATION_APPLESCRIPT_H_

#import <Cocoa/Cocoa.h>

#import "chrome/browser/chrome_browser_application_mac.h"

@class BookmarkFolderAppleScript;
@class WindowAppleScript;

@interface BrowserCrApplication (AppleScriptAdditions)

- (NSArray*)appleScriptWindows;

- (void)insertInAppleScriptWindows:(WindowAppleScript*)aWindow;

- (void)insertInAppleScriptWindows:(WindowAppleScript*)aWindow
                           atIndex:(int)index;

- (void)removeFromAppleScriptWindowsAtIndex:(int)index;

- (NSScriptObjectSpecifier*)objectSpecifier;

- (BookmarkFolderAppleScript*)otherBookmarks;

- (BookmarkFolderAppleScript*)bookmarksBar;

- (NSArray*)bookmarkFolders;

- (void)insertInBookmarksFolders:(id)aBookmarkFolder;
- (void)insertInBookmarksFolders:(id)aBookmarkFolder atIndex:(int)index;
- (void)removeFromBookmarksFoldersAtIndex:(int)index;

@end

#endif
