// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_CHROME_EVENT_PROCESSING_WINDOW_H_
#define CHROME_BROWSER_UI_COCOA_CHROME_EVENT_PROCESSING_WINDOW_H_
#pragma once

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_nsobject.h"

@interface ChromeEventProcessingWindow : NSWindow {
 @private
  BOOL redispatchingEvent_;
  BOOL eventHandled_;
}

- (BOOL)redispatchKeyEvent:(NSEvent*)event;


- (BOOL)handleExtraWindowKeyboardShortcut:(NSEvent*)event;

- (BOOL)handleDelayedWindowKeyboardShortcut:(NSEvent*)event;

- (BOOL)handleExtraBrowserKeyboardShortcut:(NSEvent*)event;

- (BOOL)performKeyEquivalent:(NSEvent*)theEvent;

@end

#endif  
