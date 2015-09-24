// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

@protocol BookmarkButtonControllerProtocol;
@class BookmarkBarFolderController;

@interface BookmarkBarFolderView : NSView {
 @private
  BOOL inDrag_;  
  BOOL dropIndicatorShown_;
  CGFloat dropIndicatorPosition_;  
  
  
  BookmarkBarFolderController* controller_;
}
- (id<BookmarkButtonControllerProtocol>)controller;
@end

@interface BookmarkBarFolderView()  
@property(assign) BOOL dropIndicatorShown;
@property(readonly) CGFloat dropIndicatorPosition;
- (void)setController:(id)controller;
@end
