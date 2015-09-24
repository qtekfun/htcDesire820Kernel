// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_CONSTRAINED_WINDOW_CONSTRAINED_WINDOW_SHEET_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_CONSTRAINED_WINDOW_CONSTRAINED_WINDOW_SHEET_CONTROLLER_H_

#import <Cocoa/Cocoa.h>
#include <vector>

#include "base/mac/scoped_nsobject.h"
#include "base/memory/scoped_vector.h"

@protocol ConstrainedWindowSheet;

@interface ConstrainedWindowSheetController : NSObject {
 @private
  base::scoped_nsobject<NSMutableArray> sheets_;
  base::scoped_nsobject<NSWindow> parentWindow_;
  base::scoped_nsobject<NSView> activeView_;
}

+ (ConstrainedWindowSheetController*)
    controllerForParentWindow:(NSWindow*)parentWindow;

+ (ConstrainedWindowSheetController*)
    controllerForSheet:(id<ConstrainedWindowSheet>)sheet;

+ (id<ConstrainedWindowSheet>)sheetForOverlayWindow:(NSWindow*)overlayWindow;

- (void)showSheet:(id<ConstrainedWindowSheet>)sheet
    forParentView:(NSView*)parentView;

- (NSPoint)originForSheet:(id<ConstrainedWindowSheet>)sheet
           withWindowSize:(NSSize)size;

- (void)closeSheet:(id<ConstrainedWindowSheet>)sheet;

- (void)parentViewDidBecomeActive:(NSView*)parentView;

- (void)pulseSheet:(id<ConstrainedWindowSheet>)sheet;

- (int)sheetCount;

@end

#endif  
