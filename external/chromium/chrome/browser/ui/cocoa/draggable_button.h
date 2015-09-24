// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

@interface DraggableButton : NSButton {
 @private
  BOOL draggable_;        
  BOOL actionHasFired_;   
  BOOL actsOnMouseDown_;  
                          
  NSTimeInterval durationMouseWasDown_;
  NSTimeInterval whenMouseDown_;
}

@property NSTimeInterval durationMouseWasDown;

@property NSTimeInterval whenMouseDown;

@property(nonatomic) BOOL actionHasFired;

@property(nonatomic) BOOL draggable;

@property(nonatomic) BOOL actsOnMouseDown;

- (void)beginDrag:(NSEvent*)dragEvent;


- (void)secondaryMouseUpAction:(BOOL)wasInside;

- (BOOL)deltaIndicatesConclusionReachedWithXDelta:(float)xDelta
                                           yDelta:(float)yDelta
                                      xHysteresis:(float)xHysteresis
                                      yHysteresis:(float)yHysteresis;

- (BOOL)deltaIndicatesDragStartWithXDelta:(float)xDelta
                                   yDelta:(float)yDelta
                              xHysteresis:(float)xHysteresis
                              yHysteresis:(float)yHysteresis;


@end  

@interface DraggableButton (Private)

- (void)endDrag;

- (void)performMouseDownAction:(NSEvent*)theEvent;


@end  
