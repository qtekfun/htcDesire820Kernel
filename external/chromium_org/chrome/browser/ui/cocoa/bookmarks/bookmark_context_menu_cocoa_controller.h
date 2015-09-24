// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_CONTEXT_MENU_COCOA_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_CONTEXT_MENU_COCOA_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#include "base/basictypes.h"
#include "base/mac/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"

@class BookmarkBarController;
class BookmarkContextMenuController;
class BookmarkContextMenuDelegateBridge;
class BookmarkModel;
class BookmarkNode;
@class MenuController;

@interface BookmarkContextMenuCocoaController : NSObject {
 @private
  
  
  
  BookmarkBarController* bookmarkBarController_;

  
  scoped_ptr<BookmarkContextMenuDelegateBridge> bridge_;

  
  
  const BookmarkNode* bookmarkNode_;

  
  
  scoped_ptr<BookmarkContextMenuController> bookmarkContextMenuController_;

  
  
  base::scoped_nsobject<MenuController> menuController_;
}

- (id)initWithBookmarkBarController:(BookmarkBarController*)controller;

- (NSMenu*)menuForBookmarkNode:(const BookmarkNode*)node;

- (NSMenu*)menuForBookmarkBar;

- (void)cancelTracking;

@end

#endif  
