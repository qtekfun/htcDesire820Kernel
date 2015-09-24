// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BROWSER_WINDOW_UTILS_H_
#define CHROME_BROWSER_UI_COCOA_BROWSER_WINDOW_UTILS_H_

#import <Cocoa/Cocoa.h>

#include "chrome/browser/ui/cocoa/themed_window.h"

class Browser;

namespace content {
struct NativeWebKeyboardEvent;
}

@interface BrowserWindowUtils : NSObject

+ (BOOL)shouldHandleKeyboardEvent:(const content::NativeWebKeyboardEvent&)event;

+ (int)getCommandId:(const content::NativeWebKeyboardEvent&)event;

+ (BOOL)handleKeyboardEvent:(NSEvent*)event
                   inWindow:(NSWindow*)window;

+ (NSString*)scheduleReplaceOldTitle:(NSString*)oldTitle
                        withNewTitle:(NSString*)newTitle
                           forWindow:(NSWindow*)window;

+ (NSPoint)themeImagePositionFor:(NSView*)windowView
                    withTabStrip:(NSView*)tabStripView
                       alignment:(ThemeImageAlignment)alignment;

+ (NSPoint)themeImagePositionInTabStripCoords:(NSView*)tabStripView
                                    alignment:(ThemeImageAlignment)alignment;

+ (void)activateWindowForController:(NSWindowController*)controller;
@end

#endif  
