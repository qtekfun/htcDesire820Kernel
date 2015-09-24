// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_NEW_TAB_BUTTON
#define CHROME_BROWSER_UI_COCOA_NEW_TAB_BUTTON
#pragma once

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_nsobject.h"


@interface NewTabButton : NSButton {
 @private
  scoped_nsobject<NSBezierPath> imagePath_;
}

- (BOOL)pointIsOverButton:(NSPoint)point;
@end

#endif  
