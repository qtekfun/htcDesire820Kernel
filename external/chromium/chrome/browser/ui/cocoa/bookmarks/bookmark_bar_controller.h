// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_BAR_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_BAR_CONTROLLER_H_
#pragma once

#import <Cocoa/Cocoa.h>
#include <map>

#include "base/memory/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/cocoa/bookmarks/bookmark_bar_bridge.h"
#import "chrome/browser/ui/cocoa/bookmarks/bookmark_bar_constants.h"
#import "chrome/browser/ui/cocoa/bookmarks/bookmark_bar_state.h"
#import "chrome/browser/ui/cocoa/bookmarks/bookmark_bar_toolbar_view.h"
#import "chrome/browser/ui/cocoa/bookmarks/bookmark_button.h"
#include "chrome/browser/ui/cocoa/tabs/tab_strip_model_observer_bridge.h"
#import "content/common/chrome_application_mac.h"
#include "webkit/glue/window_open_disposition.h"

@class BookmarkBarController;
@class BookmarkBarFolderController;
@class BookmarkBarView;
@class BookmarkButton;
@class BookmarkButtonCell;
@class BookmarkFolderTarget;
class BookmarkModel;
@class BookmarkMenu;
class BookmarkNode;
class Browser;
class GURL;

namespace bookmarks {


const CGFloat kDefaultBookmarkWidth = 150.0;

const CGFloat kBookmarkHorizontalPadding = 1.0;

const CGFloat kBookmarkVerticalPadding = 2.0;

const CGFloat kBookmarkMenuButtonMinimumWidth = 100.0;
const CGFloat kBookmarkMenuButtonMaximumWidth = 485.0;

const CGFloat kBookmarkHorizontalScreenPadding = 8.0;

const CGFloat kScrollViewContentWidthMargin = 2;

const CGFloat kBookmarkMenuOverlap = 2.0;

const CGFloat kScrollWindowVerticalMargin = 6.0;

const CGFloat kBookmarkBarMenuOffset = 2.0;

const CGFloat kBookmarkBarButtonOffset = 2.0;

const NSTimeInterval kHoverOpenDelay = 0.3;

const NSTimeInterval kDragHoverOpenDelay = 0.7;

const NSTimeInterval kDragHoverCloseDelay = 0.4;

}  

@protocol BookmarkBarControllerDelegate

- (void)bookmarkBar:(BookmarkBarController*)controller
 didChangeFromState:(bookmarks::VisualState)oldState
            toState:(bookmarks::VisualState)newState;

- (void)bookmarkBar:(BookmarkBarController*)controller
willAnimateFromState:(bookmarks::VisualState)oldState
            toState:(bookmarks::VisualState)newState;

@end

@interface BookmarkBarController :
    NSViewController<BookmarkBarState,
                     BookmarkBarToolbarViewController,
                     BookmarkButtonDelegate,
                     BookmarkButtonControllerProtocol,
                     CrApplicationEventHookProtocol,
                     NSUserInterfaceValidations> {
 @private
  
  
  
  
  bookmarks::VisualState visualState_;

  
  
  bookmarks::VisualState lastVisualState_;

  Browser* browser_;              
  BookmarkModel* bookmarkModel_;  
                                  

  
  CGFloat initialWidth_;

  
  
  
  
  int32 seedId_;
  std::map<int32,int64> menuTagMap_;

  
  scoped_nsobject<NSMutableArray> buttons_;

  
  scoped_nsobject<NSImage> folderImage_;

  
  scoped_nsobject<NSImage> defaultImage_;

  
  
  BOOL barIsEnabled_;

  
  
  scoped_ptr<BookmarkBarBridge> bridge_;

  
  id<BookmarkBarControllerDelegate> delegate_;  

  
  id<ViewResizer> resizeDelegate_;  

  
  scoped_nsobject<BookmarkFolderTarget> folderTarget_;

  
  
  
  BookmarkBarFolderController* folderController_;

  
  
  BOOL watchingForExitEvent_;

  IBOutlet BookmarkBarView* buttonView_;  
  IBOutlet BookmarkButton* offTheSideButton_;  
  IBOutlet NSMenu* buttonContextMenu_;

  NSRect originalNoItemsRect_;  
  NSRect originalImportBookmarksRect_;  

  
  scoped_nsobject<BookmarkButton> otherBookmarksButton_;

  
  
  scoped_nsobject<BookmarkMenu> buttonFolderContextMenu_;

  
  
  
  
  
  scoped_nsobject<BookmarkButton> hoverButton_;

  
  
  
  CGFloat savedFrameWidth_;

  
  
  
  
  
  
  
  int displayedButtonCount_;

  
  
  
  
  
  BOOL showFolderMenus_;

  
  
  BOOL ignoreAnimations_;

  
  BOOL hasInsertionPos_;

  
  
  CGFloat insertionPos_;
}

@property(readonly, nonatomic) bookmarks::VisualState visualState;
@property(readonly, nonatomic) bookmarks::VisualState lastVisualState;
@property(assign, nonatomic) id<BookmarkBarControllerDelegate> delegate;

- (id)initWithBrowser:(Browser*)browser
         initialWidth:(CGFloat)initialWidth
             delegate:(id<BookmarkBarControllerDelegate>)delegate
       resizeDelegate:(id<ViewResizer>)resizeDelegate;

- (void)updateAndShowNormalBar:(BOOL)showNormalBar
               showDetachedBar:(BOOL)showDetachedBar
                 withAnimation:(BOOL)animate;

- (void)updateVisibility;

- (void)setBookmarkBarEnabled:(BOOL)enabled;

- (CGFloat)getDesiredToolbarHeightCompression;

- (CGFloat)toolbarDividerOpacity;

- (void)layoutSubviews;

- (void)viewDidMoveToWindow;

- (IBAction)importBookmarks:(id)sender;

- (NSImage*)faviconForNode:(const BookmarkNode*)node;

- (void)closeFolderAndStopTrackingMenus;

- (BOOL)canEditBookmark:(const BookmarkNode*)node;

- (BOOL)canEditBookmarks;

- (IBAction)openBookmark:(id)sender;
- (IBAction)openBookmarkFolderFromButton:(id)sender;
- (IBAction)openOffTheSideFolderFromButton:(id)sender;
- (IBAction)openBookmarkInNewForegroundTab:(id)sender;
- (IBAction)openBookmarkInNewWindow:(id)sender;
- (IBAction)openBookmarkInIncognitoWindow:(id)sender;
- (IBAction)editBookmark:(id)sender;
- (IBAction)cutBookmark:(id)sender;
- (IBAction)copyBookmark:(id)sender;
- (IBAction)pasteBookmark:(id)sender;
- (IBAction)deleteBookmark:(id)sender;
- (IBAction)openAllBookmarks:(id)sender;
- (IBAction)openAllBookmarksNewWindow:(id)sender;
- (IBAction)openAllBookmarksIncognitoWindow:(id)sender;
- (IBAction)addPage:(id)sender;
- (IBAction)addFolder:(id)sender;

@end

@interface BookmarkBarController(BridgeRedirect)
- (void)loaded:(BookmarkModel*)model;
- (void)beingDeleted:(BookmarkModel*)model;
- (void)nodeAdded:(BookmarkModel*)model
           parent:(const BookmarkNode*)oldParent index:(int)index;
- (void)nodeChanged:(BookmarkModel*)model
               node:(const BookmarkNode*)node;
- (void)nodeMoved:(BookmarkModel*)model
        oldParent:(const BookmarkNode*)oldParent oldIndex:(int)oldIndex
        newParent:(const BookmarkNode*)newParent newIndex:(int)newIndex;
- (void)nodeRemoved:(BookmarkModel*)model
             parent:(const BookmarkNode*)oldParent index:(int)index;
- (void)nodeFaviconLoaded:(BookmarkModel*)model
                     node:(const BookmarkNode*)node;
- (void)nodeChildrenReordered:(BookmarkModel*)model
                         node:(const BookmarkNode*)node;
@end

@interface BookmarkBarController(InternalOrTestingAPI)
- (void)openBookmarkFolder:(id)sender;
- (BookmarkBarView*)buttonView;
- (NSMutableArray*)buttons;
- (NSButton*)offTheSideButton;
- (BOOL)offTheSideButtonIsHidden;
- (BookmarkButton*)otherBookmarksButton;
- (BookmarkBarFolderController*)folderController;
- (id)folderTarget;
- (int)displayedButtonCount;
- (void)openURL:(GURL)url disposition:(WindowOpenDisposition)disposition;
- (void)clearBookmarkBar;
- (BookmarkButtonCell*)cellForBookmarkNode:(const BookmarkNode*)node;
- (NSRect)frameForBookmarkButtonFromCell:(NSCell*)cell xOffset:(int*)xOffset;
- (void)checkForBookmarkButtonGrowth:(NSButton*)button;
- (void)frameDidChange;
- (int64)nodeIdFromMenuTag:(int32)tag;
- (int32)menuTagFromNodeId:(int64)menuid;
- (const BookmarkNode*)nodeFromMenuItem:(id)sender;
- (void)updateTheme:(ui::ThemeProvider*)themeProvider;
- (BookmarkButton*)buttonForDroppingOnAtPoint:(NSPoint)point;
- (BOOL)isEventAnExitEvent:(NSEvent*)event;
- (BOOL)shrinkOrHideView:(NSView*)view forMaxX:(CGFloat)maxViewX;

- (NSMenu *)menuForFolderNode:(const BookmarkNode*)node;
- (NSMenu*)buttonContextMenu;
- (void)setButtonContextMenu:(id)menu;
- (void)setIgnoreAnimations:(BOOL)ignore;
@end

#endif  
