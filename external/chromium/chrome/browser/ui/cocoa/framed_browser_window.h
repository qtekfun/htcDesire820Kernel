// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_FRAMED_BROWSER_WINDOW_H_
#define CHROME_BROWSER_UI_COCOA_FRAMED_BROWSER_WINDOW_H_
#pragma once

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_nsobject.h"
#include "chrome/browser/ui/cocoa/chrome_browser_window.h"

const NSInteger kFramedWindowButtonsWithTabStripOffsetFromTop = 11;
const NSInteger kFramedWindowButtonsWithTabStripOffsetFromLeft = 11;

const NSInteger kFramedWindowButtonsWithoutTabStripOffsetFromTop = 4;
const NSInteger kFramedWindowButtonsWithoutTabStripOffsetFromLeft = 8;

const NSInteger kFramedWindowButtonsInterButtonSpacing = 7;

@interface FramedBrowserWindow : ChromeBrowserWindow {
 @private
  BOOL shouldHideTitle_;
  BOOL hasTabStrip_;
  NSButton* closeButton_;
  NSButton* miniaturizeButton_;
  NSButton* zoomButton_;
}

- (void)setShouldHideTitle:(BOOL)flag;

@end

@interface NSWindow (UndocumentedAPI)

-(BOOL)_isTitleHidden;

@end

#endif  
