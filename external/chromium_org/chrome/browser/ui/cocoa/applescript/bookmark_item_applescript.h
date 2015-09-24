// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_APPLESCRIPT_BOOKMARK_ITEM_APPLESCRIPT_H_
#define CHROME_BROWSER_UI_COCOA_APPLESCRIPT_BOOKMARK_ITEM_APPLESCRIPT_H_

#import <Cocoa/Cocoa.h>

#import "chrome/browser/ui/cocoa/applescript/bookmark_node_applescript.h"

@interface BookmarkItemAppleScript : BookmarkNodeAppleScript {
 @private
  
  
  
  NSString* tempURL_;
}

- (void)setBookmarkNode:(const BookmarkNode*)aBookmarkNode;

- (NSString*)URL;

- (void)setURL:(NSString*)aURL;

@end

#endif  
