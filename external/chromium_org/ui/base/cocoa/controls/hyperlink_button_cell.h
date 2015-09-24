// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_COCOA_CONTROLS_HYPERLINK_BUTTON_CELL_H_
#define UI_BASE_COCOA_CONTROLS_HYPERLINK_BUTTON_CELL_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#include "ui/base/ui_export.h"

UI_EXPORT
@interface HyperlinkButtonCell : NSButtonCell {
  base::scoped_nsobject<NSColor> textColor_;
  BOOL shouldUnderline_;
  BOOL underlineOnHover_;
  BOOL mouseIsInside_;
}
@property(nonatomic, retain) NSColor* textColor;
@property(nonatomic, assign) BOOL underlineOnHover;
@property(nonatomic, assign) BOOL shouldUnderline;

+ (NSColor*)defaultTextColor;

+ (NSButton*)buttonWithString:(NSString*)string;

@end

@interface HyperlinkButtonCell (ExposedForTesting)
- (NSDictionary*)linkAttributes;
@end

#endif  
