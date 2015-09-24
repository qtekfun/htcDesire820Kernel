// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_BAR_FOLDER_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_BAR_FOLDER_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#import "chrome/browser/ui/cocoa/bookmarks/bookmark_button.h"
#import "ui/base/cocoa/tracking_area.h"

class Profile;

@class BookmarkBarController;
@class BookmarkBarFolderView;
@class BookmarkBarFolderHoverState;
@class BookmarkBarFolderWindow;
@class BookmarkBarFolderWindowContentView;
@class BookmarkFolderTarget;

@interface BookmarkBarFolderController :
    NSWindowController<BookmarkButtonDelegate,
                       BookmarkButtonControllerProtocol> {
 @private
  
  base::scoped_nsobject<BookmarkButton> parentButton_;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

  
  
  base::scoped_nsobject<BookmarkBarFolderController> parentController_;

  
  BookmarkBarController* barController_;  

  
  base::scoped_nsobject<NSMutableArray> buttons_;

  
  IBOutlet NSScrollView* scrollView_;

  
  IBOutlet BookmarkBarFolderWindowContentView* visibleView_;

  
  IBOutlet BookmarkBarFolderView* folderView_;

  
  
  
  
  
  
  
  
  
  IBOutlet BookmarkBarFolderWindow* shadowWindow_;

  
  
  
  IBOutlet NSView* scrollDownArrowView_;  
  IBOutlet NSView* scrollUpArrowView_;  

  
  
  BOOL subFolderGrowthToRight_;

  
  
  BookmarkButton* buttonThatMouseIsIn_;

  
  
  
  base::scoped_nsobject<BookmarkBarFolderHoverState> hoverState_;

  
  base::scoped_nsobject<BookmarkFolderTarget> folderTarget_;

  
  
  
  
  BookmarkBarFolderController* folderController_;

  
  ui::ScopedCrTrackingArea scrollTrackingArea_;

  
  
  NSTimer* scrollTimer_;

  
  
  
  CGFloat padding_;

  
  CGFloat verticalScrollDelta_;

  
  
  CGFloat verticalScrollArrowHeight_;

  
  
  BOOL ignoreAnimations_;

  
  NSScreen* screen_;

  int selectedIndex_;
  NSString* typedPrefix_;

  Profile* profile_;
}

- (id)initWithParentButton:(BookmarkButton*)button
          parentController:(BookmarkBarFolderController*)parentController
             barController:(BookmarkBarController*)barController
                   profile:(Profile*)profile;

- (BookmarkButton*)parentButton;

- (BOOL)handleInputText:(NSString*)newText;

- (void)clearInputText;

- (void)faviconLoadedForNode:(const BookmarkNode*)node;

- (void)setSelectedButtonByIndex:(int)index;

- (void)offsetFolderMenuWindow:(NSSize)offset;

- (void)reconfigureMenu;

- (void)scrollWheel:(NSEvent *)theEvent;

- (void)mouseDragged:(NSEvent*)theEvent;

@property(assign, nonatomic) BOOL subFolderGrowthToRight;

@end

@interface BookmarkBarFolderController(TestingAPI)
- (NSPoint)windowTopLeftForWidth:(int)windowWidth
                          height:(int)windowHeight;
- (NSArray*)buttons;
- (BookmarkBarFolderController*)folderController;
- (id)folderTarget;
- (void)configureWindowLevel;
- (void)performOneScroll:(CGFloat)delta;
- (BookmarkButton*)buttonThatMouseIsIn;
- (void)setIgnoreAnimations:(BOOL)ignore;

- (BOOL)canScrollUp;
- (BOOL)canScrollDown;
- (CGFloat)verticalScrollArrowHeight;
- (NSView*)visibleView;
- (NSScrollView*)scrollView;
- (NSView*)folderView;

- (IBAction)openBookmarkFolderFromButton:(id)sender;

- (BookmarkButton*)buttonForDroppingOnAtPoint:(NSPoint)point;
@end

#endif  
