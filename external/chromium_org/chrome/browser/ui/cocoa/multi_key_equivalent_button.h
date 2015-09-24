// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_MULTI_KEY_EQUIVALENT_BUTTON_H_
#define CHROME_BROWSER_UI_COCOA_MULTI_KEY_EQUIVALENT_BUTTON_H_

#import <AppKit/AppKit.h>

#include <vector>

struct KeyEquivalentAndModifierMask {
 public:
  KeyEquivalentAndModifierMask() : charCode(nil), mask(0) {}
  NSString* charCode;
  NSUInteger mask;
};


@interface MultiKeyEquivalentButton : NSButton {
 @private
  std::vector<KeyEquivalentAndModifierMask> extraKeys_;
}

- (void)addKeyEquivalent:(KeyEquivalentAndModifierMask)key;

@end

#endif  
