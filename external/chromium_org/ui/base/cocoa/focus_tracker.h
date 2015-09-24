// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#include "ui/base/ui_export.h"

UI_EXPORT
@interface FocusTracker : NSObject {
 @private
  base::scoped_nsobject<NSView> focusedView_;
}

- (id)initWithWindow:(NSWindow*)window;

- (BOOL)restoreFocusInWindow:(NSWindow*)window;
@end
