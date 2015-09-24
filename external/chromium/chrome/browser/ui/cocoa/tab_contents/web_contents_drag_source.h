// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TAB_CONTENTS_WEB_CONTENTS_DRAG_SOURCE_H_
#define CHROME_BROWSER_UI_COCOA_TAB_CONTENTS_WEB_CONTENTS_DRAG_SOURCE_H_
#pragma once

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_nsobject.h"
#include "chrome/browser/bookmarks/bookmark_node_data.h"

@class TabContentsViewCocoa;

@interface WebContentsDragSource : NSObject {
 @private
  
  TabContentsViewCocoa* contentsView_;

  
  scoped_nsobject<NSPasteboard> pasteboard_;

  
  NSDragOperation dragOperationMask_;
}

- (id)initWithContentsView:(TabContentsViewCocoa*)contentsView
                pasteboard:(NSPasteboard*)pboard
         dragOperationMask:(NSDragOperation)dragOperationMask;

- (NSImage*)dragImage;

- (void)fillPasteboard;

- (NSDragOperation)draggingSourceOperationMaskForLocal:(BOOL)isLocal;

- (void)startDrag;

- (void)endDragAt:(NSPoint)screenPoint
        operation:(NSDragOperation)operation;

- (void)moveDragTo:(NSPoint)screenPoint;

@end

#endif  
