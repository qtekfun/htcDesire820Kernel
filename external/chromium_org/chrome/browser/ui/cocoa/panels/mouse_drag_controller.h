// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_PANELS_MOUSE_DRAG_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_PANELS_MOUSE_DRAG_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

enum PanelDragState {
  PANEL_DRAG_CAN_START,  
  PANEL_DRAG_IN_PROGRESS,
  PANEL_DRAG_SUPPRESSED  
};

@class MouseDragController;

@protocol MouseDragControllerClient
- (void)prepareForDrag;

- (void)dragStarted:(NSPoint)initialMouseLocation;

- (void)dragProgress:(NSPoint)mouseLocation;

- (void)dragEnded:(BOOL)cancelled;

- (void)cleanupAfterDrag;
@end


@interface MouseDragController : NSObject {
 @private
   NSPoint initialMouseLocation_;  
   PanelDragState dragState_;
   NSView<MouseDragControllerClient>* client_;  
};

- (MouseDragController*)
    initWithClient:(NSView<MouseDragControllerClient>*)client;

- (NSView<MouseDragControllerClient>*)client;
- (NSPoint)initialMouseLocation;

- (void)mouseDown:(NSEvent*)event;
- (void)mouseDragged:(NSEvent*)event;
- (void)mouseUp:(NSEvent*)event;
@end

#endif  
