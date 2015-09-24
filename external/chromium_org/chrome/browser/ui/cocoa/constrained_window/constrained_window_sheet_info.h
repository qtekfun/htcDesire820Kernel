// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_CONSTRAINED_WINDOW_CONSTRAINED_WINDOW_SHEET_INFO_H_
#define CHROME_BROWSER_UI_COCOA_CONSTRAINED_WINDOW_CONSTRAINED_WINDOW_SHEET_INFO_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"

@protocol ConstrainedWindowSheet;

@interface ConstrainedWindowSheetInfo : NSObject {
 @private
  base::scoped_nsprotocol<id<ConstrainedWindowSheet>> sheet_;
  base::scoped_nsobject<NSView> parentView_;
  base::scoped_nsobject<NSWindow> overlayWindow_;
  BOOL sheetDidShow_;
}

@property(nonatomic, readonly) id<ConstrainedWindowSheet> sheet;
@property(nonatomic, readonly) NSView* parentView;
@property(nonatomic, readonly) NSWindow* overlayWindow;
@property(nonatomic, assign) BOOL sheetDidShow;

- (id)initWithSheet:(id<ConstrainedWindowSheet>)sheet
         parentView:(NSView*)parentView
      overlayWindow:(NSWindow*)overlayWindow;

- (void)hideSheet;

- (void)showSheet;

@end

#endif  
