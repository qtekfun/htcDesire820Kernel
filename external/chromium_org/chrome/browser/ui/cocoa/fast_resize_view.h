// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_FAST_RESIZE_VIEW_H_
#define CHROME_BROWSER_UI_COCOA_FAST_RESIZE_VIEW_H_

#import <Cocoa/Cocoa.h>

@interface FastResizeView : NSView {
 @private
  BOOL fastResizeMode_;
}

- (void)setFastResizeMode:(BOOL)fastResizeMode;

@end

#endif  
