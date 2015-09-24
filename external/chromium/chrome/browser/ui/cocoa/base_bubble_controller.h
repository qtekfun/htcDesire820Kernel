// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#import "base/mac/cocoa_protocols.h"
#include "base/memory/scoped_ptr.h"

namespace BaseBubbleControllerInternal {
class Bridge;
}

@class InfoBubbleView;

@interface BaseBubbleController : NSWindowController<NSWindowDelegate> {
 @private
  NSWindow* parentWindow_;  
  NSPoint anchor_;
  IBOutlet InfoBubbleView* bubble_;  
  
  scoped_ptr<BaseBubbleControllerInternal::Bridge> base_bridge_;
}

@property(nonatomic, readonly) NSWindow* parentWindow;
@property(nonatomic, assign) NSPoint anchorPoint;
@property(nonatomic, readonly) InfoBubbleView* bubble;

- (id)initWithWindowNibPath:(NSString*)nibPath
               parentWindow:(NSWindow*)parentWindow
                 anchoredAt:(NSPoint)anchoredAt;


- (id)initWithWindowNibPath:(NSString*)nibPath
             relativeToView:(NSView*)view
                     offset:(NSPoint)offset;


- (id)initWithWindow:(NSWindow*)theWindow
        parentWindow:(NSWindow*)parentWindow
          anchoredAt:(NSPoint)anchoredAt;

@end
