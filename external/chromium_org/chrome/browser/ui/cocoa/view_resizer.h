// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_VIEW_RESIZER_H_
#define CHROME_BROWSER_UI_COCOA_VIEW_RESIZER_H_

#include "chrome/browser/ui/tabs/tab_strip_model.h"

#import <Cocoa/Cocoa.h>

@protocol ViewResizer <NSObject>
- (void)resizeView:(NSView*)view newHeight:(CGFloat)height;

@optional
- (void)setAnimationInProgress:(BOOL)inProgress;
@end

#endif  
