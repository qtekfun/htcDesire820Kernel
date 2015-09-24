// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_ANIMATABLE_IMAGE_H_
#define CHROME_BROWSER_UI_COCOA_ANIMATABLE_IMAGE_H_

#import <Cocoa/Cocoa.h>
#import <QuartzCore/QuartzCore.h>

#include "base/mac/scoped_nsobject.h"

@interface AnimatableImage : NSWindow {
 @private
  
  base::scoped_nsobject<NSImage> image_;

  
  
  CGRect startFrame_;
  CGRect endFrame_;

  
  CGFloat startOpacity_;
  CGFloat endOpacity_;

  
  CGFloat duration_;
}

@property(nonatomic) CGRect startFrame;
@property(nonatomic) CGRect endFrame;
@property(nonatomic) CGFloat startOpacity;
@property(nonatomic) CGFloat endOpacity;
@property(nonatomic) CGFloat duration;

- (id)initWithImage:(NSImage*)image
     animationFrame:(NSRect)animationFrame;

- (void)startAnimation;

@end

#endif  
