// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_APPLESCRIPT_BOOKMARK_NODE_APPLESCRIPT_H_
#define CHROME_BROWSER_UI_COCOA_APPLESCRIPT_BOOKMARK_NODE_APPLESCRIPT_H_

#import <Cocoa/Cocoa.h>

#import "chrome/browser/ui/cocoa/applescript/element_applescript.h"

class BookmarkModel;
class BookmarkNode;

@interface BookmarkNodeAppleScript : ElementAppleScript {
 @protected
  const BookmarkNode* bookmarkNode_;  
  
  
  
  NSString* tempTitle_;
}

- (id)init;

- (id)initWithBookmarkNode:(const BookmarkNode*)aBookmarkNode;

- (void)setBookmarkNode:(const BookmarkNode*)aBookmarkNode;

- (NSString*)title;
- (void)setTitle:(NSString*)aTitle;

- (NSNumber*)index;

- (BookmarkModel*)bookmarkModel;

@end

#endif  
