// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#import "chrome/browser/ui/cocoa/bookmarks/bookmark_button.h"

@interface BookmarkBarFolderHoverState : NSObject {
 @private
  
  
  
  
  
  
  
  
  enum HoverState {
    kHoverStateClosed = 0,
    kHoverStateOpening = 1,
    kHoverStateOpen = 2,
    kHoverStateClosing = 3
  };

  
  
  
  base::scoped_nsobject<BookmarkButton> hoverButton_;

  
  
  
  HoverState hoverState_;
}

- (id)init;

- (NSDragOperation)draggingEnteredButton:(BookmarkButton*)button;

- (void)draggingExited;

@end

@interface BookmarkBarFolderHoverState(PrivateAPI)
- (void)scheduleCloseBookmarkFolderOnHoverButton;
- (void)cancelPendingCloseBookmarkFolderOnHoverButton;
- (void)scheduleOpenBookmarkFolderOnHoverButton:(BookmarkButton*)hoverButton;
- (void)cancelPendingOpenBookmarkFolderOnHoverButton;
@end

@interface BookmarkBarFolderHoverState(TestingAPI)
- (BookmarkButton*)hoverButton;
- (HoverState)hoverState;
@end
