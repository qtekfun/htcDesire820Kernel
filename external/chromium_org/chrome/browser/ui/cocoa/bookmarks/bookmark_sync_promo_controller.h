// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_SYNC_PROMO_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_SYNC_PROMO_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"

class Browser;
@class HyperlinkTextView;

@interface BookmarkSyncPromoController : NSViewController<NSTextViewDelegate> {
 @private
  
  Browser* browser_;  

  
  
  base::scoped_nsobject<HyperlinkTextView> textView_;
}

@property(nonatomic, readonly) CGFloat borderWidth;

- (id)initWithBrowser:(Browser*)browser;

- (CGFloat)preferredHeightForWidth:(CGFloat)width;

@end

#endif  
