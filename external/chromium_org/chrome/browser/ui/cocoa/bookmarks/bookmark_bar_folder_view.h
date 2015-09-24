// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#import "base/mac/scoped_nsobject.h"

@protocol BookmarkButtonControllerProtocol;
@class BookmarkBarFolderController;

@interface BookmarkBarFolderView : NSView {
 @private
  BOOL inDrag_;  
  BOOL dropIndicatorShown_;
  
  
  id<BookmarkButtonControllerProtocol> controller_;
  base::scoped_nsobject<NSBox> dropIndicator_;
}
@end
