// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_nsobject.h"
#include "chrome/browser/ui/cocoa/hover_button.h"

@interface HoverCloseButton : HoverButton {
 @private
  
  scoped_nsobject<NSBezierPath> xPath_;

  
  scoped_nsobject<NSBezierPath> circlePath_;

  
  scoped_nsobject<NSAffineTransform> transform_;

  
  
  NSSize oldSize_;
}

- (void)commonInit;

@end
