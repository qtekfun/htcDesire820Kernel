// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_COCOA_CONTROLS_HOVER_IMAGE_MENU_BUTTON_CELL_H_
#define UI_BASE_COCOA_CONTROLS_HOVER_IMAGE_MENU_BUTTON_CELL_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#include "ui/base/ui_export.h"

UI_EXPORT
@interface HoverImageMenuButtonCell : NSPopUpButtonCell {
 @private
  base::scoped_nsobject<NSImage> hoverImage_;
  BOOL hovered_;
}

@property(retain, nonatomic) NSImage* hoverImage;
@property(assign, nonatomic, getter=isHovered) BOOL hovered;

- (NSImage*)imageToDraw;

- (void)setDefaultImage:(NSImage*)defaultImage;

@end

#endif  
