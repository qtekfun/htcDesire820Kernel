// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_MENU_COCOA_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_MENU_COCOA_CONTROLLER_H_
#pragma once

#import <Cocoa/Cocoa.h>

#import "base/mac/cocoa_protocols.h"
#include "webkit/glue/window_open_disposition.h"

class BookmarkNode;
class BookmarkMenuBridge;

@interface BookmarkMenuCocoaController : NSObject<NSMenuDelegate> {
 @private
  BookmarkMenuBridge* bridge_;  
}

@property(nonatomic, readonly) NSMenu* menu;

+ (NSString*)menuTitleForNode:(const BookmarkNode*)node;

- (id)initWithBridge:(BookmarkMenuBridge *)bridge;

- (IBAction)openBookmarkMenuItem:(id)sender;
- (IBAction)openAllBookmarks:(id)sender;
- (IBAction)openAllBookmarksNewWindow:(id)sender;
- (IBAction)openAllBookmarksIncognitoWindow:(id)sender;

@end  


@interface BookmarkMenuCocoaController (ExposedForUnitTests)
- (const BookmarkNode*)nodeForIdentifier:(int)identifier;
- (void)openURLForNode:(const BookmarkNode*)node;
- (void)openAll:(NSInteger)tag
    withDisposition:(WindowOpenDisposition)disposition;
@end  

#endif  
