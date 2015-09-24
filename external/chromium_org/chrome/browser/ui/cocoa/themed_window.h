// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_THEMED_WINDOW_H_
#define CHROME_BROWSER_UI_COCOA_THEMED_WINDOW_H_

#import <Cocoa/Cocoa.h>

namespace ui {
class ThemeProvider;
}
using ui::ThemeProvider;

enum {
  THEMED_NORMAL    = 0,
  THEMED_INCOGNITO = 1 << 0,
  THEMED_POPUP     = 1 << 1,
  THEMED_DEVTOOLS  = 1 << 2
};
typedef NSUInteger ThemedWindowStyle;

enum ThemeImageAlignment {
  
  
  THEME_IMAGE_ALIGN_WITH_FRAME,
  
  
  THEME_IMAGE_ALIGN_WITH_TAB_STRIP
};


@interface NSWindow (ThemeProvider)
- (ThemeProvider*)themeProvider;
- (ThemedWindowStyle)themedWindowStyle;

- (NSPoint)themeImagePositionForAlignment:(ThemeImageAlignment)alignment;
@end

#endif  
