// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TABS_TAB_VIEW_H_
#define CHROME_BROWSER_UI_COCOA_TABS_TAB_VIEW_H_
#pragma once

#import <Cocoa/Cocoa.h>
#include <ApplicationServices/ApplicationServices.h>

#include <map>

#include "base/memory/scoped_nsobject.h"
#import "chrome/browser/ui/cocoa/background_gradient_view.h"
#import "chrome/browser/ui/cocoa/hover_close_button.h"

namespace tabs {


enum AlertState {
  kAlertNone = 0,  
  kAlertRising,
  kAlertHolding,
  kAlertFalling
};

}  

@class TabController, TabWindowController;


@interface TabView : BackgroundGradientView {
 @private
  IBOutlet TabController* controller_;
  
  
  IBOutlet HoverCloseButton* closeButton_;

  BOOL closing_;

  
  scoped_nsobject<NSTrackingArea> closeTrackingArea_;

  BOOL isMouseInside_;  
  tabs::AlertState alertState_;

  CGFloat hoverAlpha_;  
  NSTimeInterval hoverHoldEndTime_;  

  CGFloat alertAlpha_;  
  NSTimeInterval alertHoldEndTime_;  

  NSTimeInterval lastGlowUpdate_;  

  NSPoint hoverPoint_;  

  
  
  BOOL moveWindowOnDrag_;  
  BOOL tabWasDragged_;  
  BOOL draggingWithinTabStrip_;  
  BOOL chromeIsVisible_;

  NSTimeInterval tearTime_;  
  NSPoint tearOrigin_;  
  NSPoint dragOrigin_;  
  
  
  TabWindowController* sourceController_;  
  NSWindow* sourceWindow_;  
  NSRect sourceWindowFrame_;
  NSRect sourceTabFrame_;

  TabWindowController* draggedController_;  
  NSWindow* dragWindow_;  
  NSWindow* dragOverlay_;  
  
  
  
  
  
  std::map<CGWindowID, int> workspaceIDCache_;

  TabWindowController* targetController_;  
  NSCellStateValue state_;
}

@property(assign, nonatomic) NSCellStateValue state;
@property(assign, nonatomic) CGFloat hoverAlpha;
@property(assign, nonatomic) CGFloat alertAlpha;

@property(assign, nonatomic, getter=isClosing) BOOL closing;

- (void)setTrackingEnabled:(BOOL)enabled;

- (void)startAlert;

- (void)cancelAlert;

@end

@interface TabView (TabControllerInterface)
- (void)setController:(TabController*)controller;
@end

#endif  
