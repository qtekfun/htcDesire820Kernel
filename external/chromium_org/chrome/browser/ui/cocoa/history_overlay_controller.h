// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_OVERLAY_PANEL_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_OVERLAY_PANEL_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"

@class HistoryOverlayView;

enum HistoryOverlayMode {
  kHistoryOverlayModeBack,
  kHistoryOverlayModeForward
};

@interface HistoryOverlayController : NSViewController {
 @private
  HistoryOverlayMode mode_;
  
  base::scoped_nsobject<HistoryOverlayView> contentView_;
  
  base::scoped_nsobject<NSView> parent_;
}

- (id)initForMode:(HistoryOverlayMode)mode;

- (void)showPanelForView:(NSView*)view;

- (void)setProgress:(CGFloat)gestureAmount finished:(BOOL)finished;

- (void)dismiss;

@end

#endif  
