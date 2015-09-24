// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_CONFIRM_QUIT_PANEL_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_CONFIRM_QUIT_PANEL_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#import "base/mac/cocoa_protocols.h"
#include "ui/base/models/accelerator_cocoa.h"

@class ConfirmQuitFrameView;

namespace confirm_quit {

enum ConfirmQuitMetric {
  
  kNoConfirm = 0,
  
  kHoldDuration,
  
  kDoubleTap,
  
  kTapHold,

  kSampleCount
};

void RecordHistogram(ConfirmQuitMetric sample);

}  

@interface ConfirmQuitPanelController : NSWindowController<NSWindowDelegate> {
 @private
  
  ConfirmQuitFrameView* contentView_;  
}

+ (ConfirmQuitPanelController*)sharedController;

+ (BOOL)eventTriggersFeature:(NSEvent*)event;

- (NSApplicationTerminateReply)runModalLoopForApplication:(NSApplication*)app;

- (void)showWindow:(id)sender;

- (void)dismissPanel;

+ (ui::AcceleratorCocoa)quitAccelerator;

+ (NSString*)keyCommandString;

@end

@interface ConfirmQuitPanelController (UnitTesting)
+ (NSString*)keyCombinationForAccelerator:(const ui::AcceleratorCocoa&)item;
@end

#endif  
