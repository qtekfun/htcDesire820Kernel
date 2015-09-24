// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_nsobject.h"
#include "chrome/browser/ui/cocoa/hover_button.h"

@interface HoverImageButton : HoverButton {
 @private
  float defaultOpacity_;
  float hoverOpacity_;
  float pressedOpacity_;

  scoped_nsobject<NSImage> defaultImage_;
  scoped_nsobject<NSImage> hoverImage_;
  scoped_nsobject<NSImage> pressedImage_;
}

- (void)setDefaultImage:(NSImage*)image;

- (void)setHoverImage:(NSImage*)image;

- (void)setPressedImage:(NSImage*)image;

- (void)setDefaultOpacity:(float)opacity;

- (void)setHoverOpacity:(float)opacity;

- (void)setPressedOpacity:(float)opacity;

@end
