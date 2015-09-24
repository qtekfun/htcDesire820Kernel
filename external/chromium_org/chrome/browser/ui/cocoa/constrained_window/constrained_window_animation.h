// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_CONSTRAINED_WINDOW_CONSTRAINED_WINDOW_ANIMATION_H_
#define CHROME_BROWSER_UI_COCOA_CONSTRAINED_WINDOW_CONSTRAINED_WINDOW_ANIMATION_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"

@interface ConstrainedWindowAnimationBase : NSAnimation {
 @protected
  base::scoped_nsobject<NSWindow> window_;
}

- (id)initWithWindow:(NSWindow*)window;

@end

@interface ConstrainedWindowAnimationShow : ConstrainedWindowAnimationBase
@end

@interface ConstrainedWindowAnimationHide : ConstrainedWindowAnimationBase
@end

@interface ConstrainedWindowAnimationPulse : ConstrainedWindowAnimationBase
@end

#endif  
