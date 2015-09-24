// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

@class StyledTextFieldCell;

@interface StyledTextField : NSTextField {
}

- (void)resetFieldEditorFrameIfNeeded;

- (CGFloat)availableDecorationWidth;

@end

@interface StyledTextField (ExposedForTesting)
- (StyledTextFieldCell*)styledTextFieldCell;
@end
