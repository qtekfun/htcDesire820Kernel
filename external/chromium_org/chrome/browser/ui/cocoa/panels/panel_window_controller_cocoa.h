// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_PANELS_PANEL_WINDOW_CONTROLLER_COCOA_H_
#define CHROME_BROWSER_UI_COCOA_PANELS_PANEL_WINDOW_CONTROLLER_COCOA_H_


#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#import "chrome/browser/ui/cocoa/chrome_browser_window.h"
#include "chrome/browser/ui/panels/panel.h"
#import "ui/base/cocoa/tracking_area.h"

class PanelCocoa;
@class PanelTitlebarViewCocoa;

@interface PanelWindowCocoaImpl : ChromeBrowserWindow {
}
@end

@interface PanelWindowControllerCocoa : NSWindowController
                                            <NSWindowDelegate,
                                             NSAnimationDelegate> {
 @private
  IBOutlet PanelTitlebarViewCocoa* titlebar_view_;
  scoped_ptr<PanelCocoa> windowShim_;
  base::scoped_nsobject<NSString> pendingWindowTitle_;
  NSViewAnimation* boundsAnimation_;  
                                      
  BOOL animateOnBoundsChange_;
  BOOL throbberShouldSpin_;
  BOOL playingMinimizeAnimation_;
  float animationStopToShowTitlebarOnly_;
  BOOL canBecomeKeyWindow_;
  
  
  
  
  BOOL activationRequestedByPanel_;
  
  BOOL userResizing_;
  
  ui::ScopedCrTrackingArea trackingArea_;
}

- (id)initWithPanel:(PanelCocoa*)window;

- (Panel*)panel;

- (void)webContentsInserted:(content::WebContents*)contents;
- (void)webContentsDetached:(content::WebContents*)contents;

- (void)updateWebContentsViewFrame;

- (void)disableWebContentsViewAutosizing;
- (void)enableWebContentsViewAutosizing;

- (void)revealAnimatedWithFrame:(const NSRect&)frame;

- (void)updateTitleBar;
- (void)updateIcon;
- (void)updateThrobber:(BOOL)shouldSpin;
- (void)updateTitleBarMinimizeRestoreButtonVisibility;

- (void)closePanel;

- (void)minimizeButtonClicked:(int)modifierFlags;
- (void)restoreButtonClicked:(int)modifierFlags;

- (void)setPanelFrame:(NSRect)frame
              animate:(BOOL)animate;

- (void)startDrag:(NSPoint)mouseLocation;
- (void)endDrag:(BOOL)cancelled;
- (void)drag:(NSPoint)mouseLocation;

- (PanelTitlebarViewCocoa*)titlebarView;
- (int)titlebarHeightInScreenCoordinates;

- (void)onTitlebarMouseClicked:(int)modifierFlags;

- (void)onTitlebarDoubleClicked:(int)modifierFlags;

- (void)animationDidEnd:(NSAnimation*)animation;
- (void)terminateBoundsAnimation;

- (BOOL)isAnimatingBounds;

- (void)activate;
- (void)deactivate;

- (void)preventBecomingKeyWindow:(BOOL)prevent;

- (void)fullScreenModeChanged:(bool)isFullScreen;

- (BOOL)canBecomeKeyWindow;

- (BOOL)activationRequestedByPanel;

- (void)updateWindowLevel;
- (void)updateWindowLevel:(BOOL)panelIsMinimized;

- (void)updateWindowCollectionBehavior;

- (void)updateTrackingArea;

- (void)showShadow:(BOOL)show;

- (void)miniaturize;
- (BOOL)isMiniaturized;

- (BOOL)canResizeByMouseAtCurrentLocation;

- (NSRect)frameRectForContentRect:(NSRect)contentRect;
- (NSRect)contentRectForFrameRect:(NSRect)frameRect;

@end  

#endif  
