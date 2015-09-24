// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_nsobject.h"


@interface FocusTracker : NSObject {
 @private
  scoped_nsobject<NSView> focusedView_;
}

- (id)initWithWindow:(NSWindow*)window;

- (BOOL)restoreFocusInWindow:(NSWindow*)window;
@end
