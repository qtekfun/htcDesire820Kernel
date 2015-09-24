// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_BAR_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_BAR_CONTROLLER_H_

#import <Cocoa/Cocoa.h>
#include <map>

#import "base/mac/cocoa_protocols.h"
#include "base/mac/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/cocoa/bookmarks/bookmark_bar_bridge.h"
#import "chrome/browser/ui/cocoa/bookmarks/bookmark_bar_constants.h"
#import "chrome/browser/ui/cocoa/bookmarks/bookmark_bar_state.h"
#import "chrome/browser/ui/cocoa/bookmarks/bookmark_bar_toolbar_view.h"
#import "chrome/browser/ui/cocoa/bookmarks/bookmark_button.h"
#include "chrome/browser/ui/cocoa/tabs/tab_strip_model_observer_bridge.h"
#include "ui/base/window_open_disposition.h"

@class BookmarkBarController;
@class BookmarkBarFolderController;
@class BookmarkBarView;
@class BookmarkButtonCell;
@class BookmarkFolderTarget;
@class BookmarkContextMenuCocoaController;
class BookmarkModel;
class BookmarkNode;
class Browser;
class GURL;
namespace ui {
class ThemeProvider;
}

namespace bookmarks {


const CGFloat kDefaultBookmarkWidth = 150.0;

const CGFloat kBookmarkHorizontalPadding = 1.0;

const CGFloat kBookmarkVerticalPadding = 2.0;

const CGFloat kBookmarkLeftMargin = 2.0;

const CGFloat kBookmarkRightMargin = 2.0;

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
 didChangeFromState:(BookmarkBar::State)oldState
            toState:(BookmarkBar::State)newState;

- (void)bookmarkBar:(BookmarkBarController*)controller
willAnimateFromState:(BookmarkBar::State)oldState
            toState:(BookmarkBar::State)newState;

@end

@interface BookmarkBarController :
    NSViewController<BookmarkBarState,
                     BookmarkBarToolbarViewController,
                     BookmarkButtonDelegate,
                     BookmarkButtonControllerProtocol,
                     NSDraggingDestination> {
 @private
  
  
  BookmarkBar::State currentState_;

  
  BookmarkBar::State lastState_;

  
  BOOL isAnimationRunning_;

  Browser* browser_;              
  BookmarkModel* bookmarkModel_;  
                                  

  
  CGFloat initialWidth_;

  
  
  
  
  int32 seedId_;
  std::map<int32,int64> menuTagMap_;

  
  base::scoped_nsobject<NSMutableArray> buttons_;

  
  base::scoped_nsobject<NSImage> folderImage_;

  
  base::scoped_nsobject<NSImage> defaultImage_;

  
  
  BOOL barIsEnabled_;

  
  
  scoped_ptr<BookmarkBarBridge> bridge_;

  
  id<BookmarkBarControllerDelegate> delegate_;  

  
  id<ViewResizer> resizeDelegate_;  

  
  base::scoped_nsobject<BookmarkFolderTarget> folderTarget_;

  
  
  
  BookmarkBarFolderController* folderController_;

  
  
  id exitEventTap_;

  IBOutlet BookmarkBarView* buttonView_;  
  IBOutlet BookmarkButton* offTheSideButton_;  

  NSRect originalNoItemsRect_;  
  NSRect originalImportBookmarksRect_;  

  
  base::scoped_nsobject<BookmarkButton> otherBookmarksButton_;

  
  base::scoped_nsobject<BookmarkButton> appsPageShortcutButton_;

  
  
  
  
  
  base::scoped_nsobject<BookmarkButton> hoverButton_;

  
  
  
  CGFloat savedFrameWidth_;

  
  
  
  
  
  
  
  int displayedButtonCount_;

  
  
  
  
  
  BOOL showFolderMenus_;

  
  
  BOOL stateAnimationsEnabled_;

  
  
  BOOL innerContentAnimationsEnabled_;

  
  BOOL hasInsertionPos_;

  
  
  CGFloat insertionPos_;

  
  base::scoped_nsobject<BookmarkContextMenuCocoaController>
      contextMenuController_;
}

@property(readonly, nonatomic) BookmarkBar::State currentState;
@property(readonly, nonatomic) BookmarkBar::State lastState;
@property(readonly, nonatomic) BOOL isAnimationRunning;
@property(assign, nonatomic) id<BookmarkBarControllerDelegate> delegate;
@property(assign, nonatomic) BOOL stateAnimationsEnabled;
@property(assign, nonatomic) BOOL innerContentAnimationsEnabled;

- (id)initWithBrowser:(Browser*)browser
         initialWidth:(CGFloat)initialWidth
             delegate:(id<BookmarkBarControllerDelegate>)delegate
       resizeDelegate:(id<ViewResizer>)resizeDelegate;

- (Browser*)browser;

- (BookmarkContextMenuCocoaController*)menuController;

- (void)updateState:(BookmarkBar::State)newState
         changeType:(BookmarkBar::AnimateChangeType)changeType;

- (void)updateVisibility;

- (void)updateAppsPageShortcutButtonVisibility;

- (void)updateHiddenState;

- (void)setBookmarkBarEnabled:(BOOL)enabled;

- (CGFloat)getDesiredToolbarHeightCompression;

- (CGFloat)toolbarDividerOpacity;

- (void)layoutSubviews;

- (void)viewDidMoveToWindow;

- (NSImage*)faviconForNode:(const BookmarkNode*)node;

- (void)closeFolderAndStopTrackingMenus;

- (BOOL)canEditBookmark:(const BookmarkNode*)node;

- (BOOL)canEditBookmarks;

- (IBAction)openBookmark:(id)sender;
- (IBAction)openBookmarkFolderFromButton:(id)sender;
- (IBAction)openOffTheSideFolderFromButton:(id)sender;
- (IBAction)importBookmarks:(id)sender;
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
- (NSButton*)appsPageShortcutButton;
- (BOOL)offTheSideButtonIsHidden;
- (BOOL)appsPageShortcutButtonIsHidden;
- (BookmarkButton*)otherBookmarksButton;
- (BookmarkBarFolderController*)folderController;
- (id)folderTarget;
- (int)displayedButtonCount;
- (void)openURL:(GURL)url disposition:(WindowOpenDisposition)disposition;
- (void)clearBookmarkBar;
- (BookmarkButtonCell*)cellForBookmarkNode:(const BookmarkNode*)node;
- (BookmarkButtonCell*)cellForCustomButtonWithText:(NSString*)text
                                             image:(NSImage*)image;
- (NSRect)frameForBookmarkButtonFromCell:(NSCell*)cell xOffset:(int*)xOffset;
- (void)checkForBookmarkButtonGrowth:(NSButton*)button;
- (void)frameDidChange;
- (int64)nodeIdFromMenuTag:(int32)tag;
- (int32)menuTagFromNodeId:(int64)menuid;
- (void)updateTheme:(ui::ThemeProvider*)themeProvider;
- (BookmarkButton*)buttonForDroppingOnAtPoint:(NSPoint)point;
- (BOOL)isEventAnExitEvent:(NSEvent*)event;
- (BOOL)shrinkOrHideView:(NSView*)view forMaxX:(CGFloat)maxViewX;
- (void)unhighlightBookmark:(const BookmarkNode*)node;

- (NSMenu *)menuForFolderNode:(const BookmarkNode*)node;
@end

#endif  
