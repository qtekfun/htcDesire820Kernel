// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TOOLBAR_RELOAD_BUTTON_H_
#define CHROME_BROWSER_UI_COCOA_TOOLBAR_RELOAD_BUTTON_H_

#import <Cocoa/Cocoa.h>

#import "chrome/browser/ui/cocoa/image_button_cell.h"
#import "chrome/browser/ui/cocoa/toolbar/toolbar_button.h"


@interface ReloadButton : ToolbarButton<ImageButton> {
 @private
  
  NSTimer* pendingReloadTimer_;
}

- (void)updateTag:(NSInteger)anInt;

- (void)setIsLoading:(BOOL)isLoading force:(BOOL)force;

@end

#endif  
