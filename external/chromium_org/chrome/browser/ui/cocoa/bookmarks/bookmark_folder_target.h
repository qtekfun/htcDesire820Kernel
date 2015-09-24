// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_FOLDER_TARGET_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_FOLDER_TARGET_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

@class BookmarkButton;
@protocol BookmarkButtonControllerProtocol;
class BookmarkNode;
class Profile;

@interface BookmarkFolderTarget : NSObject {
  
  id<BookmarkButtonControllerProtocol> controller_;  
  Profile* profile_;
}

- (id)initWithController:(id<BookmarkButtonControllerProtocol>)controller
                 profile:(Profile*)profile;

- (IBAction)openBookmarkFolderFromButton:(id)sender;

- (void)fillPasteboard:(NSPasteboard*)pboard
       forDragOfButton:(BookmarkButton*)button;

@end

extern NSString* kBookmarkButtonDragType;

#endif  
