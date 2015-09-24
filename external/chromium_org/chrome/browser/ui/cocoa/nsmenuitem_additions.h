// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_NSMENUITEM_ADDITIONS_H_
#define CHROME_BROWSER_UI_COCOA_NSMENUITEM_ADDITIONS_H_

#import <Cocoa/Cocoa.h>

@interface NSMenuItem(ChromeAdditions)

- (BOOL)cr_firesForKeyEvent:(NSEvent*)event;

- (BOOL)cr_firesForKeyEventIfEnabled:(NSEvent*)event;

@end

#endif  
