// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#import "chrome/browser/ui/cocoa/styled_text_field_cell.h"

#include "base/mac/scoped_nsobject.h"

@interface FindBarTextFieldCell : StyledTextFieldCell {
 @private
  
  base::scoped_nsobject<NSAttributedString> resultsString_;
}

- (void)setActiveMatch:(NSInteger)current of:(NSInteger)total;

- (void)clearResults;

- (NSString*)resultsString;

@end
