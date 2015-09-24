// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_NEW_TAB_BUTTON
#define CHROME_BROWSER_UI_COCOA_NEW_TAB_BUTTON

#import <Cocoa/Cocoa.h>


@interface NewTabButton : NSButton

- (BOOL)pointIsOverButton:(NSPoint)point;
@end

#endif  
