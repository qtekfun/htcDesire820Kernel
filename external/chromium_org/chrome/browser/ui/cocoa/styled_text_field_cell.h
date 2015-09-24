// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_STYLED_TEXT_FIELD_CELL_H_
#define CHROME_BROWSER_UI_COCOA_STYLED_TEXT_FIELD_CELL_H_

#import <Cocoa/Cocoa.h>

#import "chrome/browser/ui/cocoa/rect_path_utils.h"


@interface StyledTextFieldCell : NSTextFieldCell {
}

@end

@interface StyledTextFieldCell (ProtectedMethods)

- (NSRect)textCursorFrameForFrame:(NSRect)cellFrame;

- (NSRect)textFrameForFrame:(NSRect)cellFrame;

- (CGFloat)topTextFrameOffset;

- (CGFloat)bottomTextFrameOffset;

- (CGFloat)cornerRadius;

- (rect_path_utils::RoundedCornerFlags)roundedFlags;

- (BOOL)shouldDrawBezel;

@end

#endif  
