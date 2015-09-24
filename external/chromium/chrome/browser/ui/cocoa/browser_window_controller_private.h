// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BROWSER_WINDOW_CONTROLLER_PRIVATE_H_
#define CHROME_BROWSER_UI_COCOA_BROWSER_WINDOW_CONTROLLER_PRIVATE_H_
#pragma once

#import "chrome/browser/ui/cocoa/browser_window_controller.h"


@interface BrowserWindowController(Private)

- (void)createTabStripController;

- (void)saveWindowPositionIfNeeded;

- (void)saveWindowPositionToPrefs:(PrefService*)prefs;

- (NSRect)window:(NSWindow*)window
    willPositionSheet:(NSWindow*)sheet
            usingRect:(NSRect)defaultSheetRect;

- (void)layoutSubviews;

- (CGFloat)floatingBarHeight;

- (CGFloat)layoutTabStripAtMaxY:(CGFloat)maxY
                          width:(CGFloat)width
                     fullscreen:(BOOL)fullscreen;

- (CGFloat)layoutToolbarAtMinX:(CGFloat)minX
                          maxY:(CGFloat)maxY
                         width:(CGFloat)width;

- (BOOL)placeBookmarkBarBelowInfoBar;

- (CGFloat)layoutBookmarkBarAtMinX:(CGFloat)minX
                              maxY:(CGFloat)maxY
                             width:(CGFloat)width;

- (void)layoutFloatingBarBackingView:(NSRect)frame
                          fullscreen:(BOOL)fullscreen;

- (CGFloat)layoutInfoBarAtMinX:(CGFloat)minX
                          maxY:(CGFloat)maxY
                         width:(CGFloat)width;

- (CGFloat)layoutDownloadShelfAtMinX:(CGFloat)minX
                                minY:(CGFloat)minY
                               width:(CGFloat)width;

- (void)layoutTabContentArea:(NSRect)frame;

- (BOOL)shouldShowBookmarkBar;

- (BOOL)shouldShowDetachedBookmarkBar;

- (void)adjustToolbarAndBookmarkBarForCompression:(CGFloat)compression;

- (void)adjustUIForFullscreen:(BOOL)fullscreen;

- (void)enableBarVisibilityUpdates;
- (void)disableBarVisibilityUpdates;

- (void)setUpOSFullScreenButton;

@end  


#endif  
