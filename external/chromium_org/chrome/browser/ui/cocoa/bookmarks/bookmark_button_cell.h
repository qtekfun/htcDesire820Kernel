// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_BUTTON_CELL_H_
#define CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_BUTTON_CELL_H_

#import "chrome/browser/ui/cocoa/gradient_button_cell.h"

@class BookmarkContextMenuCocoaController;
class BookmarkNode;

@interface BookmarkButtonCell : GradientButtonCell<NSMenuDelegate> {
 @private
  
  
  BookmarkContextMenuCocoaController* menuController_;

  BOOL empty_;  

  
  int startingChildIndex_;

  
  
  BOOL drawFolderArrow_;

  
  base::scoped_nsobject<NSImage> arrowImage_;

  
  base::scoped_nsobject<NSColor> textColor_;
}

@property(nonatomic, readwrite, assign) const BookmarkNode* bookmarkNode;
@property(nonatomic, readwrite, assign) int startingChildIndex;
@property(nonatomic, readwrite, assign) BOOL drawFolderArrow;

+ (id)buttonCellForNode:(const BookmarkNode*)node
                   text:(NSString*)text
                  image:(NSImage*)image
         menuController:(BookmarkContextMenuCocoaController*)menuController;

+ (id)buttonCellWithText:(NSString*)text
                   image:(NSImage*)image
          menuController:(BookmarkContextMenuCocoaController*)menuController;

- (id)initForNode:(const BookmarkNode*)node
             text:(NSString*)text
            image:(NSImage*)image
   menuController:(BookmarkContextMenuCocoaController*)menuController;

- (id)initWithText:(NSString*)text
             image:(NSImage*)image
    menuController:(BookmarkContextMenuCocoaController*)menuController;

- (BOOL)empty;
- (void)setEmpty:(BOOL)empty;

- (void)setBookmarkCellText:(NSString*)title
                      image:(NSImage*)image;

- (void)setTextColor:(NSColor*)color;

- (BOOL)isFolderButtonCell;

@end

#endif  
