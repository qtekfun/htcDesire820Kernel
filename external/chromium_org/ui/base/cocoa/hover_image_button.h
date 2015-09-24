// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_COCOA_HOVER_IMAGE_BUTTON_H_
#define UI_BASE_COCOA_HOVER_IMAGE_BUTTON_H_

#import <Cocoa/Cocoa.h>

#import "base/mac/scoped_nsobject.h"
#import "ui/base/cocoa/hover_button.h"
#include "ui/base/ui_export.h"

UI_EXPORT
@interface HoverImageButton : HoverButton {
 @private
  base::scoped_nsobject<NSImage> defaultImage_;
  base::scoped_nsobject<NSImage> hoverImage_;
  base::scoped_nsobject<NSImage> pressedImage_;
}

- (void)setDefaultImage:(NSImage*)image;

- (void)setHoverImage:(NSImage*)image;

- (void)setPressedImage:(NSImage*)image;

@end

#endif  
