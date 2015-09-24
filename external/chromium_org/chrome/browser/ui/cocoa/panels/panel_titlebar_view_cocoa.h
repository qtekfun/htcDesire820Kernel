// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_PANELS_PANEL_TITLEBAR_VIEW_COCOA_H_
#define CHROME_BROWSER_UI_COCOA_PANELS_PANEL_TITLEBAR_VIEW_COCOA_H_

#import <Cocoa/Cocoa.h>

#import "chrome/browser/ui/cocoa/panels/mouse_drag_controller.h"
#import "ui/base/cocoa/tracking_area.h"

@class HoverImageButton;
@class MouseDragController;
@class PanelWindowControllerCocoa;


@interface PanelTitlebarOverlayView : NSView {
 @private
  IBOutlet PanelWindowControllerCocoa* controller_;
  BOOL disableReordering_;
}
@end

@interface RepaintAnimation : NSAnimation {
 @private
  NSView* targetView_;
}
- (id)initWithView:(NSView*)targetView duration:(double) duration;
- (void)setCurrentProgress:(NSAnimationProgress)progress;
@end

@interface PanelTitlebarViewCocoa : NSView
                                   <NSAnimationDelegate,
                                    MouseDragControllerClient> {
 @private
  IBOutlet PanelWindowControllerCocoa* controller_;
  IBOutlet NSView* icon_;
  IBOutlet NSTextField* title_;
  IBOutlet HoverImageButton* minimizeButton_;
  IBOutlet HoverImageButton* restoreButton_;
  IBOutlet HoverImageButton* customCloseButton_;
  
  
  IBOutlet NSView* overlay_;
  NSButton* closeButton_;  
                           
  ui::ScopedCrTrackingArea closeButtonTrackingArea_;
  BOOL isDrawingAttention_;

  
  base::scoped_nsobject<RepaintAnimation> glintAnimation_;
  base::scoped_nsobject<NSTimer> glintAnimationTimer_;
  int glintCounter_;

  
  base::scoped_nsobject<MouseDragController> dragController_;
}

- (void)onCloseButtonClick:(id)sender;
- (void)onMinimizeButtonClick:(id)sender;
- (void)onRestoreButtonClick:(id)sender;

- (void)attach;

- (void)setTitle:(NSString*)newTitle;
- (void)setIcon:(NSView*)newIcon;

- (NSView*)icon;

- (void)setMinimizeButtonVisibility:(BOOL)visible;
- (void)setRestoreButtonVisibility:(BOOL)visible;

- (void)updateCustomButtonsLayout;
- (void)updateIconAndTitleLayout;

- (void)didChangeFrame:(NSNotification*)notification;
- (void)didChangeMainWindow:(NSNotification*)notification;

- (void)drawAttention;
- (void)stopDrawingAttention;
- (BOOL)isDrawingAttention;
- (void)startGlintAnimation;
- (void)restartGlintAnimation:(NSTimer*)timer;
- (void)stopGlintAnimation;

@end  

@interface PanelTitlebarViewCocoa(TestingAPI)

- (PanelWindowControllerCocoa*)controller;

- (NSTextField*)title;
- (NSButton*)closeButton;
- (NSButton*)minimizeButton;
- (NSButton*)restoreButton;

- (void)simulateCloseButtonClick;

- (void)pressLeftMouseButtonTitlebar:(NSPoint)mouseLocation
                           modifiers:(int)modifierFlags;
- (void)releaseLeftMouseButtonTitlebar:(int)modifierFlags;
- (void)dragTitlebar:(NSPoint)mouseLocation;
- (void)cancelDragTitlebar;
- (void)finishDragTitlebar;

@end  

#endif  
