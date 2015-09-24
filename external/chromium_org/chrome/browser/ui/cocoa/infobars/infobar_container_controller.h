// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_INFOBARS_INFOBAR_CONTAINER_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_INFOBARS_INFOBAR_CONTAINER_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#import "chrome/browser/ui/cocoa/view_resizer.h"

@class BrowserWindowController;
@class InfoBarController;
class InfoBarCocoa;
class InfoBarContainerCocoa;
class InfoBarDelegate;
class TabStripModel;

namespace content {
class WebContents;
}

@protocol InfoBarContainerControllerBase
- (BrowserWindowController*)browserWindowController;
- (BOOL)shouldSuppressTopInfoBarTip;
- (CGFloat)infobarArrowX;
@end

@interface InfoBarContainerController
    : NSViewController<InfoBarContainerControllerBase> {
 @private
  
  id<ViewResizer> resizeDelegate_;  

  
  content::WebContents* currentWebContents_;  

  
  base::scoped_nsobject<NSMutableArray> infobarControllers_;

  
  scoped_ptr<InfoBarContainerCocoa> containerCocoa_;

  
  BOOL shouldSuppressTopInfoBarTip_;

  
  BOOL isAnimating_;

  
  
  
  int oldOverlappingTipHeight_;
}

@property(nonatomic, assign) BOOL shouldSuppressTopInfoBarTip;

- (id)initWithResizeDelegate:(id<ViewResizer>)resizeDelegate;

- (void)changeWebContents:(content::WebContents*)contents;

- (void)tabDetachedWithContents:(content::WebContents*)contents;

- (CGFloat)overlappingTipHeight;

- (void)addInfoBar:(InfoBarCocoa*)infobar
          position:(NSUInteger)position;

- (void)removeInfoBar:(InfoBarCocoa*)infobar;

- (void)positionInfoBarsAndRedraw:(BOOL)isAnimating;

@end

#endif  
