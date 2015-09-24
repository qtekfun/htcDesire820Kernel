// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_BUTTON_CELL_H_
#define CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_BUTTON_CELL_H_
#pragma once

#import "base/mac/cocoa_protocols.h"
#import "chrome/browser/ui/cocoa/gradient_button_cell.h"

class BookmarkNode;

@interface BookmarkButtonCell : GradientButtonCell<NSMenuDelegate> {
 @private
  BOOL empty_;  

  
  int startingChildIndex_;

  
  
  BOOL drawFolderArrow_;

  
  scoped_nsobject<NSImage> arrowImage_;
}

@property(nonatomic, readwrite, assign) const BookmarkNode* bookmarkNode;
@property(nonatomic, readwrite, assign) int startingChildIndex;
@property(nonatomic, readwrite, assign) BOOL drawFolderArrow;

+ (id)buttonCellForNode:(const BookmarkNode*)node
            contextMenu:(NSMenu*)contextMenu
               cellText:(NSString*)cellText
              cellImage:(NSImage*)cellImage;

- (id)initForNode:(const BookmarkNode*)node
      contextMenu:(NSMenu*)contextMenu
         cellText:(NSString*)cellText
        cellImage:(NSImage*)cellImage;

- (BOOL)empty;  
- (void)setEmpty:(BOOL)empty;

- (void)setBookmarkCellText:(NSString*)title
                      image:(NSImage*)image;

- (void)setTextColor:(NSColor*)color;

- (BOOL)isFolderButtonCell;

@end

#endif  
