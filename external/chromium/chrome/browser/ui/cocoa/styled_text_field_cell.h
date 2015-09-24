// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_STYLED_TEXT_FIELD_CELL_H_
#define CHROME_BROWSER_UI_COCOA_STYLED_TEXT_FIELD_CELL_H_
#pragma once

#import <Cocoa/Cocoa.h>

typedef enum {
  StyledTextFieldCellRoundedAll = 0,
  StyledTextFieldCellRoundedLeft = 1
} StyledTextFieldCellRoundedFlags;


@interface StyledTextFieldCell : NSTextFieldCell {
}

@end

@interface StyledTextFieldCell (ProtectedMethods)

- (NSRect)textCursorFrameForFrame:(NSRect)cellFrame;

- (NSRect)textFrameForFrame:(NSRect)cellFrame;

- (CGFloat)baselineAdjust;

- (CGFloat)cornerRadius;

- (StyledTextFieldCellRoundedFlags)roundedFlags;

- (BOOL)shouldDrawBezel;

@end

#endif  
