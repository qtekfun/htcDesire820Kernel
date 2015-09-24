// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BROWSER_WINDOW_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_BROWSER_WINDOW_CONTROLLER_H_


#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#import "chrome/browser/ui/cocoa/bookmarks/bookmark_bar_controller.h"
#import "chrome/browser/ui/cocoa/bookmarks/bookmark_bubble_controller.h"
#import "chrome/browser/ui/cocoa/browser_command_executor.h"
#import "chrome/browser/ui/cocoa/fullscreen_exit_bubble_controller.h"
#import "chrome/browser/ui/cocoa/tabs/tab_strip_controller.h"
#import "chrome/browser/ui/cocoa/tabs/tab_window_controller.h"
#import "chrome/browser/ui/cocoa/themed_window.h"
#import "chrome/browser/ui/cocoa/url_drop_target.h"
#import "chrome/browser/ui/cocoa/view_resizer.h"
#include "ui/gfx/rect.h"

@class AvatarButtonController;
class Browser;
class BrowserWindow;
class BrowserWindowCocoa;
@class DevToolsController;
@class DownloadShelfController;
class ExtensionKeybindingRegistryCocoa;
@class FindBarCocoaController;
@class FullscreenModeController;
@class FullscreenWindow;
@class InfoBarContainerController;
class LocationBarViewMac;
@class OverlayableContentsController;
@class PresentationModeController;
class StatusBubbleMac;
@class TabStripController;
@class TabStripView;
@class ToolbarController;

namespace content {
class WebContents;
}

@interface BrowserWindowController :
  TabWindowController<NSUserInterfaceValidations,
                      BookmarkBarControllerDelegate,
                      BrowserCommandExecutor,
                      ViewResizer,
                      TabStripControllerDelegate> {
 @private
  
  
  
  
  scoped_ptr<Browser> browser_;
  NSWindow* savedRegularWindow_;
  scoped_ptr<BrowserWindowCocoa> windowShim_;
  base::scoped_nsobject<ToolbarController> toolbarController_;
  base::scoped_nsobject<TabStripController> tabStripController_;
  base::scoped_nsobject<FindBarCocoaController> findBarCocoaController_;
  base::scoped_nsobject<InfoBarContainerController> infoBarContainerController_;
  base::scoped_nsobject<DownloadShelfController> downloadShelfController_;
  base::scoped_nsobject<BookmarkBarController> bookmarkBarController_;
  base::scoped_nsobject<DevToolsController> devToolsController_;
  base::scoped_nsobject<OverlayableContentsController>
      overlayableContentsController_;
  base::scoped_nsobject<PresentationModeController> presentationModeController_;
  base::scoped_nsobject<FullscreenModeController> fullscreenModeController_;
  base::scoped_nsobject<FullscreenExitBubbleController>
      fullscreenExitBubbleController_;

  
  
  
  
  StatusBubbleMac* statusBubble_;

  BookmarkBubbleController* bookmarkBubbleController_;  
  BOOL initializing_;  
  BOOL ownsBrowser_;  

  
  
  
  CGFloat windowTopGrowth_;
  CGFloat windowBottomGrowth_;

  
  
  
  
  BOOL isShrinkingFromZoomed_;

  
  
  CGFloat totalMagnifyGestureAmount_;
  NSInteger currentZoomStepDelta_;

  
  
  
  base::scoped_nsobject<AvatarButtonController> avatarButtonController_;

  
  
  
  base::scoped_nsobject<NSView> floatingBarBackingView_;

  
  
  base::scoped_nsobject<NSWindow> fullscreenWindow_;

  
  
  
  BOOL enteredPresentationModeFromFullscreen_;

  
  
  BOOL enteringFullscreen_;

  
  
  BOOL enteringPresentationMode_;

  
  
  
  NSRect savedRegularWindowFrame_;

  
  CGFloat floatingBarShownFraction_;

  
  
  
  
  
  base::scoped_nsobject<NSMutableSet> barVisibilityLocks_;

  
  
  BOOL barVisibilityUpdatesEnabled_;

  
  
  
  GURL fullscreenUrl_;
  FullscreenExitBubbleType fullscreenBubbleType_;

  
  
  scoped_ptr<ExtensionKeybindingRegistryCocoa> extension_keybinding_registry_;

  
  NSUInteger overlappedViewCount_;
}

+ (BrowserWindowController*)browserWindowControllerForWindow:(NSWindow*)window;

+ (BrowserWindowController*)browserWindowControllerForView:(NSView*)view;

+ (void)updateSigninItem:(id)signinItem
              shouldShow:(BOOL)showSigninMenuItem
          currentProfile:(Profile*)profile;

- (id)initWithBrowser:(Browser*)browser;

- (void)destroyBrowser;

- (gfx::Rect)enforceMinWindowSize:(gfx::Rect)bounds;

- (BrowserWindow*)browserWindow;

- (ToolbarController*)toolbarController;

- (TabStripController*)tabStripController;

- (FindBarCocoaController*)findBarCocoaController;

- (InfoBarContainerController*)infoBarContainerController;

- (StatusBubbleMac*)statusBubble;

- (LocationBarViewMac*)locationBarBridge;

- (NSView*)floatingBarBackingView;

- (OverlayableContentsController*)overlayableContentsController;

- (Profile*)profile;

- (AvatarButtonController*)avatarButtonController;

- (void)updateToolbarWithContents:(content::WebContents*)tab;

- (void)setStarredState:(BOOL)isStarred;

- (void)zoomChangedForActiveTab:(BOOL)canShowBubble;

- (NSRect)selectedTabGrowBoxRect;

- (void)setIsLoading:(BOOL)isLoading force:(BOOL)force;

- (void)activate;

- (void)focusLocationBar:(BOOL)selectAll;

- (void)focusTabContents;

- (NSRect)regularWindowFrame;

- (BOOL)shouldShowAvatar;

- (BOOL)isBookmarkBarVisible;

- (BOOL)isBookmarkBarAnimating;

- (BookmarkBarController*)bookmarkBarController;

- (DevToolsController*)devToolsController;

- (BOOL)isDownloadShelfVisible;

- (DownloadShelfController*)downloadShelf;

- (void)addFindBar:(FindBarCocoaController*)findBarCocoaController;

- (void)userChangedTheme;

- (void)executeCommand:(int)command;

- (BOOL)handledByExtensionCommand:(NSEvent*)event;

- (NSRect)statusBubbleBaseFrame;

- (void)showBookmarkBubbleForURL:(const GURL&)url
               alreadyBookmarked:(BOOL)alreadyBookmarked;

- (void)updateDevToolsForContents:(content::WebContents*)contents;

- (ui::ThemeProvider*)themeProvider;

- (ThemedWindowStyle)themedWindowStyle;

- (NSPoint)themeImagePositionForAlignment:(ThemeImageAlignment)alignment;

- (NSPoint)bookmarkBubblePoint;

- (void)sheetDidEnd:(NSWindow*)sheet
         returnCode:(NSInteger)code
            context:(void*)context;

- (void)onFindBarVisibilityChanged;

- (void)onOverlappedViewShown;

- (void)onOverlappedViewHidden;

@end  


@interface BrowserWindowController(WindowType)

- (BOOL)supportsWindowFeature:(int)feature;

- (BOOL)hasTitleBar;

- (BOOL)hasToolbar;

- (BOOL)hasLocationBar;

- (BOOL)supportsBookmarkBar;

- (BOOL)isTabbedWindow;

@end  


@interface BrowserWindowController(Fullscreen)

- (void)handleLionToggleFullscreen;

- (void)enterFullscreen;
- (void)exitFullscreen;

- (void)updateFullscreenExitBubbleURL:(const GURL&)url
                           bubbleType:(FullscreenExitBubbleType)bubbleType;

- (BOOL)isFullscreen;

- (void)enterPresentationModeForURL:(const GURL&)url
                         bubbleType:(FullscreenExitBubbleType)bubbleType;
- (void)exitPresentationMode;

- (void)enterFullscreenForURL:(const GURL&)url
                   bubbleType:(FullscreenExitBubbleType)bubbleType;

- (BOOL)inPresentationMode;

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


@interface BrowserWindowController (TestingAPI)

- (void)installAvatar;

- (id)initWithBrowser:(Browser*)browser takeOwnership:(BOOL)ownIt;

- (BOOL)adjustWindowHeightBy:(CGFloat)deltaH;

- (NSWindow*)createFullscreenWindow;

- (void)resetWindowGrowthState;

- (NSSize)overflowFrom:(NSRect)source
                    to:(NSRect)target;

- (FullscreenExitBubbleController*)fullscreenExitBubbleController;

- (NSRect)omniboxPopupAnchorRect;

- (void)layoutInfoBars;

@end  


#endif  
