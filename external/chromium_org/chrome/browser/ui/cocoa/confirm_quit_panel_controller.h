// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_CONFIRM_QUIT_PANEL_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_CONFIRM_QUIT_PANEL_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

@class ConfirmQuitFrameView;

namespace ui {
class PlatformAcceleratorCocoa;
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

+ (NSString*)keyCommandString;

@end

@interface ConfirmQuitPanelController (UnitTesting)
+ (NSString*)keyCombinationForAccelerator:
    (const ui::PlatformAcceleratorCocoa&)item;
@end

#endif  
