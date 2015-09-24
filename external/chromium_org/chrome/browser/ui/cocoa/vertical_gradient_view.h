// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_VERTICAL_GRADIENT_VIEW_H_
#define CHROME_BROWSER_UI_COCOA_VERTICAL_GRADIENT_VIEW_H_

#include "base/mac/scoped_nsobject.h"

#import <Cocoa/Cocoa.h>

@interface VerticalGradientView : NSView {
 @private
  
  base::scoped_nsobject<NSGradient> gradient_;
  
  base::scoped_nsobject<NSColor> strokeColor_;
}

- (NSGradient*)gradient;
- (void)setGradient:(NSGradient*)gradient;

- (NSColor*)strokeColor;
- (void)setStrokeColor:(NSColor*)gradient;

@end

#endif 
