// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_BAR_TOOLBAR_VIEW_H_
#define CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_BAR_TOOLBAR_VIEW_H_

#import <Cocoa/Cocoa.h>

#import "chrome/browser/ui/cocoa/animatable_view.h"
#import "chrome/browser/ui/cocoa/bookmarks/bookmark_bar_state.h"

@class BookmarkBarView;

class ThemeService;

@protocol BookmarkBarToolbarViewController <BookmarkBarState>
- (int)currentTabContentsHeight;

- (ThemeService*)themeService;

@end

@interface BookmarkBarToolbarView : AnimatableView {
 @private
   
   
   IBOutlet id<BookmarkBarToolbarViewController> controller_;
}
@end

#endif  
