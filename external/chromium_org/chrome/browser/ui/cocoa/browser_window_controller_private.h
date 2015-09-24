// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BROWSER_WINDOW_CONTROLLER_PRIVATE_H_
#define CHROME_BROWSER_UI_COCOA_BROWSER_WINDOW_CONTROLLER_PRIVATE_H_

#import "chrome/browser/ui/cocoa/browser_window_controller.h"

namespace browser_window_controller {

enum CoreAnimationStatus {
  kCoreAnimationDisabled,
  kCoreAnimationEnabledLazy,
  kCoreAnimationEnabledAlways,
};

}  

@interface BrowserWindowController(Private)

- (void)createTabStripController;

- (void)saveWindowPositionIfNeeded;

- (NSRect)window:(NSWindow*)window
    willPositionSheet:(NSWindow*)sheet
            usingRect:(NSRect)defaultSheetRect;

- (void)layoutSubviews;

- (CGFloat)floatingBarHeight;

- (void)showFullscreenExitBubbleIfNecessary;
- (void)destroyFullscreenExitBubbleIfNecessary;

- (CGFloat)layoutTabStripAtMaxY:(CGFloat)maxY
                          width:(CGFloat)width
                     fullscreen:(BOOL)fullscreen;

- (CGFloat)layoutToolbarAtMinX:(CGFloat)minX
                          maxY:(CGFloat)maxY
                         width:(CGFloat)width;

- (BOOL)placeBookmarkBarBelowInfoBar;

- (CGFloat)layoutBookmarkBarAtMinX:(CGFloat)minX
                              maxY:(CGFloat)maxY
                             width:(CGFloat)width;

- (void)layoutFloatingBarBackingView:(NSRect)frame
                    presentationMode:(BOOL)presentationMode;

- (CGFloat)layoutInfoBarAtMinX:(CGFloat)minX
                          maxY:(CGFloat)maxY
                         width:(CGFloat)width;

- (CGFloat)layoutDownloadShelfAtMinX:(CGFloat)minX
                                minY:(CGFloat)minY
                               width:(CGFloat)width;

- (void)layoutTabContentArea:(NSRect)frame;

- (void)adjustToolbarAndBookmarkBarForCompression:(CGFloat)compression;

- (BOOL)shouldShowPresentationModeToggle;

- (void)moveViewsForFullscreenForSnowLeopard:(BOOL)fullscreen
                               regularWindow:(NSWindow*)regularWindow
                            fullscreenWindow:(NSWindow*)fullscreenWindow;

- (void)setPresentationModeInternal:(BOOL)presentationMode
                      forceDropdown:(BOOL)forceDropdown;

- (void)enterFullscreenForSnowLeopard;
- (void)exitFullscreenForSnowLeopard;

- (void)registerForContentViewResizeNotifications;
- (void)deregisterForContentViewResizeNotifications;

- (void)adjustUIForPresentationMode:(BOOL)fullscreen;

- (void)enableBarVisibilityUpdates;
- (void)disableBarVisibilityUpdates;

- (CGFloat)toolbarDividerOpacity;

- (void)updateSubviewZOrder:(BOOL)inPresentationMode;

- (void)updateAllowOverlappingViews:(BOOL)inPresentationMode;

- (void)updateInfoBarTipVisibility;

- (browser_window_controller::CoreAnimationStatus)coreAnimationStatus;

@end  

#endif  
