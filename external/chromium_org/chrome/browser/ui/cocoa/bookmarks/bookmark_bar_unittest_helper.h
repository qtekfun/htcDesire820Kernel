// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_BAR_UNITTEST_HELPER_H_
#define CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_BAR_UNITTEST_HELPER_H_

#import <Foundation/Foundation.h>

#import "chrome/browser/ui/cocoa/bookmarks/bookmark_bar_controller.h"
#import "chrome/browser/ui/cocoa/bookmarks/bookmark_bar_folder_controller.h"
#import "chrome/browser/ui/cocoa/bookmarks/bookmark_button.h"

@interface BookmarkBarController (BookmarkBarUnitTestHelper)

- (BookmarkButton*)buttonWithTitleEqualTo:(NSString*)title;

@end


@interface BookmarkBarFolderController (BookmarkBarUnitTestHelper)

- (BookmarkButton*)buttonWithTitleEqualTo:(NSString*)title;

@end


@interface BookmarkButton (BookmarkBarUnitTestHelper)

- (NSPoint)center;

- (NSPoint)top;

- (NSPoint)bottom;

- (NSPoint)left;

- (NSPoint)right;

@end

#endif  
