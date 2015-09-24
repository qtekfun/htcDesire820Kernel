// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BROWSER_WINDOW_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_BROWSER_WINDOW_CONTROLLER_H_
#pragma once


#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/sync/sync_ui_util.h"
#import "chrome/browser/ui/cocoa/bookmarks/bookmark_bar_controller.h"
#import "chrome/browser/ui/cocoa/bookmarks/bookmark_bubble_controller.h"
#import "chrome/browser/ui/cocoa/browser_command_executor.h"
#import "chrome/browser/ui/cocoa/tab_contents/tab_contents_controller.h"
#import "chrome/browser/ui/cocoa/tabs/tab_strip_controller.h"
#import "chrome/browser/ui/cocoa/tabs/tab_window_controller.h"
#import "chrome/browser/ui/cocoa/themed_window.h"
#import "chrome/browser/ui/cocoa/url_drop_target.h"
#import "chrome/browser/ui/cocoa/view_resizer.h"


class Browser;
class BrowserWindow;
class BrowserWindowCocoa;
class ConstrainedWindowMac;
@class DevToolsController;
@class DownloadShelfController;
@class FindBarCocoaController;
@class FullscreenController;
@class GTMWindowSheetController;
@class IncognitoImageView;
@class InfoBarContainerController;
class LocationBarViewMac;
@class PreviewableContentsController;
@class SidebarController;
class StatusBubbleMac;
class TabContents;
@class TabStripController;
@class TabStripView;
@class ToolbarController;


@interface BrowserWindowController :
  TabWindowController<NSUserInterfaceValidations,
                      BookmarkBarControllerDelegate,
                      BrowserCommandExecutor,
                      ViewResizer,
                      TabContentsControllerDelegate,
                      TabStripControllerDelegate> {
 @private
  
  
  
  
  scoped_ptr<Browser> browser_;
  NSWindow* savedRegularWindow_;
  scoped_ptr<BrowserWindowCocoa> windowShim_;
  scoped_nsobject<ToolbarController> toolbarController_;
  scoped_nsobject<TabStripController> tabStripController_;
  scoped_nsobject<FindBarCocoaController> findBarCocoaController_;
  scoped_nsobject<InfoBarContainerController> infoBarContainerController_;
  scoped_nsobject<DownloadShelfController> downloadShelfController_;
  scoped_nsobject<BookmarkBarController> bookmarkBarController_;
  scoped_nsobject<DevToolsController> devToolsController_;
  scoped_nsobject<SidebarController> sidebarController_;
  scoped_nsobject<PreviewableContentsController> previewableContentsController_;
  scoped_nsobject<FullscreenController> fullscreenController_;

  
  
  
  
  StatusBubbleMac* statusBubble_;

  BookmarkBubbleController* bookmarkBubbleController_;  
  BOOL initializing_;  
  BOOL ownsBrowser_;  

  
  
  
  CGFloat windowTopGrowth_;
  CGFloat windowBottomGrowth_;

  
  
  
  
  BOOL isShrinkingFromZoomed_;

  
  
  CGFloat totalMagnifyGestureAmount_;
  NSInteger currentZoomStepDelta_;

  
  
  scoped_nsobject<IncognitoImageView> incognitoBadge_;

  
  
  
  scoped_nsobject<NSView> floatingBarBackingView_;

  
  
  BOOL floatingBarAboveBookmarkBar_;

  
  CGFloat floatingBarShownFraction_;

  
  
  
  
  
  scoped_nsobject<NSMutableSet> barVisibilityLocks_;

  
  
  BOOL barVisibilityUpdatesEnabled_;
}

+ (BrowserWindowController*)browserWindowControllerForWindow:(NSWindow*)window;

+ (BrowserWindowController*)browserWindowControllerForView:(NSView*)view;

- (id)initWithBrowser:(Browser*)browser;

- (void)destroyBrowser;

- (BrowserWindow*)browserWindow;

- (ToolbarController*)toolbarController;

- (TabStripController*)tabStripController;

- (InfoBarContainerController*)infoBarContainerController;

- (StatusBubbleMac*)statusBubble;

- (LocationBarViewMac*)locationBarBridge;

- (void)updateToolbarWithContents:(TabContents*)tab
               shouldRestoreState:(BOOL)shouldRestore;

- (void)setStarredState:(BOOL)isStarred;

- (void)setIsLoading:(BOOL)isLoading force:(BOOL)force;

- (void)activate;

- (void)focusLocationBar:(BOOL)selectAll;

- (void)focusTabContents;

- (NSRect)regularWindowFrame;

- (BOOL)isBookmarkBarVisible;

- (BOOL)isBookmarkBarAnimating;

- (void)updateBookmarkBarVisibilityWithAnimation:(BOOL)animate;

- (BOOL)isDownloadShelfVisible;

- (DownloadShelfController*)downloadShelf;

- (void)addFindBar:(FindBarCocoaController*)findBarCocoaController;

- (void)userChangedTheme;

- (void)executeCommand:(int)command;

- (NSRect)statusBubbleBaseFrame;

- (void)showBookmarkBubbleForURL:(const GURL&)url
               alreadyBookmarked:(BOOL)alreadyBookmarked;

- (GTMWindowSheetController*)sheetController;

- (void)attachConstrainedWindow:(ConstrainedWindowMac*)window;
- (void)removeConstrainedWindow:(ConstrainedWindowMac*)window;
- (BOOL)canAttachConstrainedWindow;

- (void)updateDevToolsForContents:(TabContents*)contents;

- (void)updateSidebarForContents:(TabContents*)contents;

- (ui::ThemeProvider*)themeProvider;

- (ThemedWindowStyle)themedWindowStyle;

- (NSPoint)themePatternPhase;

- (NSPoint)bookmarkBubblePoint;

- (void)toggleTabStripDisplayMode;

- (void)showInstant:(TabContents*)previewContents;
- (void)hideInstant;
- (void)commitInstant;

- (NSRect)instantFrame;

- (void)sheetDidEnd:(NSWindow*)sheet
         returnCode:(NSInteger)code
            context:(void*)context;

@end  


@interface BrowserWindowController(WindowType)

- (BOOL)supportsWindowFeature:(int)feature;

- (BOOL)hasTitleBar;

- (BOOL)hasToolbar;

- (BOOL)hasLocationBar;

- (BOOL)supportsBookmarkBar;

- (BOOL)isNormalWindow;

@end  


@interface BrowserWindowController(Fullscreen)

- (IBAction)enterFullscreen:(id)sender;

- (void)setFullscreen:(BOOL)fullscreen;

- (BOOL)isFullscreen;

- (void)resizeFullscreenWindow;

- (CGFloat)floatingBarShownFraction;
- (void)setFloatingBarShownFraction:(CGFloat)fraction;

- (BOOL)isBarVisibilityLockedForOwner:(id)owner;
- (void)lockBarVisibilityForOwner:(id)owner
                    withAnimation:(BOOL)animate
                            delay:(BOOL)delay;
- (void)releaseBarVisibilityForOwner:(id)owner
                       withAnimation:(BOOL)animate
                               delay:(BOOL)delay;

- (BOOL)floatingBarHasFocus;

- (void)openTabpose;

@end  


@interface BrowserWindowController(TestingAPI)

- (void)installIncognitoBadge;

- (id)initWithBrowser:(Browser*)browser takeOwnership:(BOOL)ownIt;

- (void)adjustWindowHeightBy:(CGFloat)deltaH;

- (NSWindow*)createFullscreenWindow;

- (void)resetWindowGrowthState;

- (NSSize)overflowFrom:(NSRect)source
                    to:(NSRect)target;
@end  


#endif  
