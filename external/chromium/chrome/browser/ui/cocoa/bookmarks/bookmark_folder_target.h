// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_FOLDER_TARGET_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_FOLDER_TARGET_CONTROLLER_H_
#pragma once

#import <Cocoa/Cocoa.h>

@class BookmarkButton;
@protocol BookmarkButtonControllerProtocol;
class BookmarkNode;

@interface BookmarkFolderTarget : NSObject {
  
  id<BookmarkButtonControllerProtocol> controller_;  
}

- (id)initWithController:(id<BookmarkButtonControllerProtocol>)controller;

- (IBAction)openBookmarkFolderFromButton:(id)sender;

- (void)copyBookmarkNode:(const BookmarkNode*)node
            toPasteboard:(NSPasteboard*)pboard;

- (void)fillPasteboard:(NSPasteboard*)pboard
       forDragOfButton:(BookmarkButton*)button;

@end

extern NSString* kBookmarkButtonDragType;

#endif  
