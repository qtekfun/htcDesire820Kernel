// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_BAR_FOLDER_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_BAR_FOLDER_CONTROLLER_H_
#pragma once

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_nsobject.h"
#import "chrome/browser/ui/cocoa/bookmarks/bookmark_button.h"
#import "chrome/browser/ui/cocoa/tracking_area.h"

@class BookmarkBarController;
@class BookmarkBarFolderView;
@class BookmarkFolderTarget;
@class BookmarkBarFolderHoverState;
@class BookmarkBarFolderWindow;
@class BookmarkBarFolderWindowContentView;

@interface BookmarkBarFolderController :
    NSWindowController<BookmarkButtonDelegate,
                       BookmarkButtonControllerProtocol,
                       NSUserInterfaceValidations> {
 @private
  
  scoped_nsobject<BookmarkButton> parentButton_;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

  
  
  scoped_nsobject<BookmarkBarFolderController> parentController_;

  
  BookmarkBarController* barController_;  

  
  scoped_nsobject<NSMutableArray> buttons_;

  
  IBOutlet NSScrollView* scrollView_;

  
  IBOutlet BookmarkBarFolderWindowContentView* visibleView_;

  
  IBOutlet BookmarkBarFolderView* folderView_;

  
  
  
  
  
  
  
  
  
  IBOutlet BookmarkBarFolderWindow* shadowWindow_;

  
  
  
  IBOutlet NSView* scrollDownArrowView_;  
  IBOutlet NSView* scrollUpArrowView_;  

  
  
  BOOL subFolderGrowthToRight_;

  
  
  BookmarkButton* buttonThatMouseIsIn_;

  
  IBOutlet NSMenu* buttonMenu_;

  
  IBOutlet NSMenu* folderMenu_;

  
  
  
  scoped_nsobject<BookmarkBarFolderHoverState> hoverState_;

  
  scoped_nsobject<BookmarkFolderTarget> folderTarget_;

  
  
  
  
  BookmarkBarFolderController* folderController_;

  
  ScopedCrTrackingArea scrollTrackingArea_;

  
  
  NSTimer* scrollTimer_;

  
  
  
  CGFloat padding_;

  
  CGFloat verticalScrollDelta_;

  
  
  CGFloat verticalScrollArrowHeight_;

  
  
  BOOL ignoreAnimations_;

  int selectedIndex_;
  NSString* typedPrefix_;
}

- (id)initWithParentButton:(BookmarkButton*)button
          parentController:(BookmarkBarFolderController*)parentController
             barController:(BookmarkBarController*)barController;

- (BookmarkButton*)parentButton;

- (BOOL)handleInputText:(NSString*)newText;

- (void)clearInputText;

- (void)faviconLoadedForNode:(const BookmarkNode*)node;

- (void)setSelectedButtonByIndex:(int)index;

- (void)offsetFolderMenuWindow:(NSSize)offset;

- (void)reconfigureMenu;

- (IBAction)cutBookmark:(id)sender;
- (IBAction)copyBookmark:(id)sender;
- (IBAction)pasteBookmark:(id)sender;
- (IBAction)deleteBookmark:(id)sender;

- (void)scrollWheel:(NSEvent *)theEvent;

- (void)mouseDragged:(NSEvent*)theEvent;


- (IBAction)addFolder:(id)sender;
- (IBAction)addPage:(id)sender;
- (IBAction)editBookmark:(id)sender;
- (IBAction)openBookmark:(id)sender;
- (IBAction)openAllBookmarks:(id)sender;
- (IBAction)openAllBookmarksIncognitoWindow:(id)sender;
- (IBAction)openAllBookmarksNewWindow:(id)sender;
- (IBAction)openBookmarkInIncognitoWindow:(id)sender;
- (IBAction)openBookmarkInNewForegroundTab:(id)sender;
- (IBAction)openBookmarkInNewWindow:(id)sender;

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

- (BookmarkButton*)buttonForDroppingOnAtPoint:(NSPoint)point;
@end

#endif  
