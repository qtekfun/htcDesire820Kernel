// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_COCOA_ITEM_DRAG_CONTROLLER_H_
#define UI_APP_LIST_COCOA_ITEM_DRAG_CONTROLLER_H_

#import <Cocoa/Cocoa.h>
#import <QuartzCore/QuartzCore.h>

#include "base/mac/scoped_nsobject.h"

@class AppsGridViewItem;

@interface ItemDragController : NSViewController {
 @private
  base::scoped_nsobject<CALayer> dragLayer_;
  base::scoped_nsobject<NSButton> buttonToRestore_;
  NSPoint mouseOffset_;
  NSTimeInterval growStart_;
  BOOL shrinking_;
}

- (id)initWithGridCellSize:(NSSize)size;

- (void)initiate:(AppsGridViewItem*)item
    mouseDownLocation:(NSPoint)mouseDownLocation
      currentLocation:(NSPoint)currentLocation
            timestamp:(NSTimeInterval)eventTimestamp;

- (void)update:(NSPoint)currentLocation
     timestamp:(NSTimeInterval)eventTimestamp;

- (void)complete:(AppsGridViewItem*)item
    targetOrigin:(NSPoint)targetOrigin;

@end

#endif  
