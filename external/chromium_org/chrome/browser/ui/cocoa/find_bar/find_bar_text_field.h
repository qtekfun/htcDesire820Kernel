// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>
#import "chrome/browser/ui/cocoa/styled_text_field.h"

@class FindBarTextFieldCell;


@interface FindBarTextField : StyledTextField {
}

- (FindBarTextFieldCell*)findBarTextFieldCell;

@end
