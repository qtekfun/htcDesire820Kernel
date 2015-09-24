// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_ANIMATABLE_VIEW_H_
#define CHROME_BROWSER_UI_COCOA_ANIMATABLE_VIEW_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#import "chrome/browser/ui/cocoa/background_gradient_view.h"
#import "chrome/browser/ui/cocoa/view_resizer.h"


@interface AnimatableView : BackgroundGradientView<NSAnimationDelegate> {
 @protected
  IBOutlet id delegate_;  

 @private
  base::scoped_nsobject<NSAnimation> currentAnimation_;
  id<ViewResizer> resizeDelegate_;  
}

@property(assign, nonatomic) id delegate;
@property(assign, nonatomic) id<ViewResizer> resizeDelegate;

- (CGFloat)height;

- (void)setHeight:(CGFloat)newHeight;

- (void)animateToNewHeight:(CGFloat)newHeight
                  duration:(NSTimeInterval)duration;

- (void)stopAnimation;

- (NSAnimationProgress)currentAnimationProgress;

@end

#endif  
