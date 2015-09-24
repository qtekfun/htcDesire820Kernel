// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_COCOA_ANIMATION_UTILS_H
#define UI_BASE_COCOA_ANIMATION_UTILS_H

#import <Cocoa/Cocoa.h>
#import <QuartzCore/QuartzCore.h>



class WithNoAnimation {
 public:
  WithNoAnimation() {
    [NSAnimationContext beginGrouping];
    [[NSAnimationContext currentContext] setDuration:0.0];
  }

  ~WithNoAnimation() {
   [NSAnimationContext endGrouping];
  }
};

class ScopedCAActionDisabler {
 public:
  ScopedCAActionDisabler() {
    [CATransaction begin];
    [CATransaction setValue:[NSNumber numberWithBool:YES]
                     forKey:kCATransactionDisableActions];
  }

  ~ScopedCAActionDisabler() {
    [CATransaction commit];
  }
};

class ScopedCAActionSetDuration {
 public:
  explicit ScopedCAActionSetDuration(NSTimeInterval duration) {
    [CATransaction begin];
    [CATransaction setValue:[NSNumber numberWithFloat:duration]
                     forKey:kCATransactionAnimationDuration];
  }

  ~ScopedCAActionSetDuration() {
    [CATransaction commit];
  }
};

#endif 
