// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BROWSER_ZOOM_BUBBLE_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_BROWSER_ZOOM_BUBBLE_CONTROLLER_H_

#include "base/mac/scoped_block.h"
#include "base/mac/scoped_nsobject.h"
#include "chrome/browser/ui/cocoa/base_bubble_controller.h"
#import "ui/base/cocoa/tracking_area.h"

namespace content {
class WebContents;
}

class ZoomBubbleControllerDelegate {
 public:
  
  virtual content::WebContents* GetWebContents() = 0;

  
  virtual void OnClose() = 0;
};

@interface ZoomBubbleController : BaseBubbleController {
 @private
  ZoomBubbleControllerDelegate* delegate_;

  
  
  BOOL autoClose_;

  
  base::scoped_nsobject<NSTextField> zoomPercent_;

  
  BOOL isMouseInside_;

  
  ui::ScopedCrTrackingArea trackingArea_;
}

- (id)initWithParentWindow:(NSWindow*)parentWindow
                  delegate:(ZoomBubbleControllerDelegate*)delegate;

- (void)showAnchoredAt:(NSPoint)anchorPoint autoClose:(BOOL)autoClose;

- (void)onZoomChanged;

- (void)resetToDefault:(id)sender;

- (void)zoomIn:(id)sender;

- (void)zoomOut:(id)sender;

- (void)closeWithoutAnimation;

@end

namespace chrome {

void SetZoomBubbleAutoCloseDelayForTesting(NSTimeInterval time_interval);

}  

#endif  
