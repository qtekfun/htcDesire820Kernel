// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_CONSTRAINED_WINDOW_CONSTRAINED_WINDOW_CUSTOM_WINDOW_H_
#define CHROME_BROWSER_UI_COCOA_CONSTRAINED_WINDOW_CONSTRAINED_WINDOW_CUSTOM_WINDOW_H_

#import <Cocoa/Cocoa.h>

#import "chrome/browser/ui/cocoa/chrome_event_processing_window.h"

@interface ConstrainedWindowCustomWindow : ChromeEventProcessingWindow

- (id)initWithContentRect:(NSRect)contentRect;

@end

@interface ConstrainedWindowCustomWindowContentView : NSView
@end

#endif  
