// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_BAR_VIEW_H_
#define CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_BAR_VIEW_H_

#import <Cocoa/Cocoa.h>

#import "third_party/GTM/AppKit/GTMUILocalizerAndLayoutTweaker.h"

@class BookmarkBarController;
@class BookmarkBarItemContainer;
@class BookmarkBarTextField;

@interface BookmarkBarView : NSView {
 @private
  BOOL dropIndicatorShown_;
  CGFloat dropIndicatorPosition_;  

  IBOutlet BookmarkBarController* controller_;
  IBOutlet BookmarkBarTextField* noItemTextfield_;
  IBOutlet NSButton* importBookmarksButton_;
  BookmarkBarItemContainer* noItemContainer_;
}
- (BookmarkBarTextField*)noItemTextfield;
- (NSButton*)importBookmarksButton;
- (BookmarkBarController*)controller;

@property(nonatomic, assign) IBOutlet BookmarkBarItemContainer* noItemContainer;
@end

@interface BookmarkBarView()  
@property(nonatomic, readonly) BOOL dropIndicatorShown;
@property(nonatomic, readonly) CGFloat dropIndicatorPosition;
- (void)setController:(id)controller;
@end


@interface BookmarkBarTextField : NSTextField {
 @private
  IBOutlet BookmarkBarView* barView_;
}
@end

@interface BookmarkBarItemContainer : GTMWidthBasedTweaker {
 @private
  IBOutlet BookmarkBarView* barView_;
}
@end

#endif  
