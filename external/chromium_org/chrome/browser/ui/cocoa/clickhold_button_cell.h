// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_CLICKHOLD_BUTTON_CELL_H_
#define CHROME_BROWSER_UI_COCOA_CLICKHOLD_BUTTON_CELL_H_

#import <Cocoa/Cocoa.h>

#import "chrome/browser/ui/cocoa/image_button_cell.h"

@interface ClickHoldButtonCell : ImageButtonCell {
 @private
  BOOL enableClickHold_;
  NSTimeInterval clickHoldTimeout_;
  id clickHoldTarget_;                  
  SEL clickHoldAction_;
  BOOL trackOnlyInRect_;
  BOOL activateOnDrag_;
}

@property(assign, nonatomic) BOOL enableClickHold;

@property(assign, nonatomic) NSTimeInterval clickHoldTimeout;

@property(assign, nonatomic) BOOL trackOnlyInRect;

@property(assign, nonatomic) BOOL activateOnDrag;

@property(assign, nonatomic) id clickHoldTarget;
@property(assign, nonatomic) SEL clickHoldAction;

@end  

#endif  
