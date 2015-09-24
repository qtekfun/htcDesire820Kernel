// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_DRAGGABLE_BUTTON_MIXIN_H_
#define CHROME_BROWSER_UI_COCOA_DRAGGABLE_BUTTON_MIXIN_H_

#import <Cocoa/Cocoa.h>


enum DraggableButtonResult {
  
  kDraggableButtonImplDidWork,
  kDraggableButtonMixinCallSuper,

  
  kDraggableButtonMixinDidWork,
  kDraggableButtonImplUseBase,
};


@protocol DraggableButtonMixin

@required

- (void)beginDrag:(NSEvent*)dragEvent;

@optional

- (DraggableButtonResult)performMouseDownAction:(NSEvent*)theEvent;

- (DraggableButtonResult)secondaryMouseUpAction:(BOOL)wasInside;

- (DraggableButtonResult)endDrag;

- (DraggableButtonResult)deltaIndicatesDragStartWithXDelta:(float)xDelta
    yDelta:(float)yDelta
    xHysteresis:(float)xHysteresis
    yHysteresis:(float)yHysteresis
    indicates:(BOOL*)result;

- (DraggableButtonResult)deltaIndicatesConclusionReachedWithXDelta:(float)xDelta
    yDelta:(float)yDelta
    xHysteresis:(float)xHysteresis
    yHysteresis:(float)yHysteresis
    indicates:(BOOL*)result;

@end


@interface DraggableButtonImpl : NSObject {
 @private
  
  NSButton<DraggableButtonMixin>* button_;

  
  BOOL draggable_;

  
  BOOL actionHasFired_;

  
  BOOL actsOnMouseDown_;

  NSTimeInterval durationMouseWasDown_;
  NSTimeInterval whenMouseDown_;
}

@property(nonatomic) NSTimeInterval durationMouseWasDown;

@property(nonatomic) NSTimeInterval whenMouseDown;

@property(nonatomic) BOOL actionHasFired;

@property(nonatomic) BOOL draggable;

@property(nonatomic) BOOL actsOnMouseDown;

- (id)initWithButton:(NSButton<DraggableButtonMixin>*)button;

- (DraggableButtonResult)mouseDownImpl:(NSEvent*)event;
- (DraggableButtonResult)mouseUpImpl:(NSEvent*)event;

@end

#endif  
