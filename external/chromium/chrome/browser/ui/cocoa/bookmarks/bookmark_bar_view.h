// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_BAR_VIEW_H_
#define CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_BAR_VIEW_H_
#pragma once

#import <Cocoa/Cocoa.h>

#import "chrome/browser/ui/cocoa/background_gradient_view.h"

@class BookmarkBarController;

@interface BookmarkBarView : BackgroundGradientView {
 @private
  BOOL dropIndicatorShown_;
  CGFloat dropIndicatorPosition_;  

  IBOutlet BookmarkBarController* controller_;
  IBOutlet NSTextField* noItemTextfield_;
  IBOutlet NSButton* importBookmarksButton_;
  NSView* noItemContainer_;
}
- (NSTextField*)noItemTextfield;
- (NSButton*)importBookmarksButton;
- (BookmarkBarController*)controller;

@property(nonatomic, assign) IBOutlet NSView* noItemContainer;
@end

@interface BookmarkBarView()  
@property(nonatomic, readonly) BOOL dropIndicatorShown;
@property(nonatomic, readonly) CGFloat dropIndicatorPosition;
- (void)setController:(id)controller;
@end

#endif  
