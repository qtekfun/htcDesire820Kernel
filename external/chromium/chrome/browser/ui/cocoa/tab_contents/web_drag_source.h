// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "base/file_path.h"
#include "base/memory/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#include "googleurl/src/gurl.h"

@class TabContentsViewCocoa;
struct WebDropData;

@interface WebDragSource : NSObject {
 @private
  
  TabContentsViewCocoa* contentsView_;

  
  scoped_ptr<WebDropData> dropData_;

  
  scoped_nsobject<NSImage> dragImage_;

  
  NSPoint imageOffset_;

  
  scoped_nsobject<NSPasteboard> pasteboard_;

  
  NSDragOperation dragOperationMask_;

  
  FilePath downloadFileName_;

  
  GURL downloadURL_;
}

- (id)initWithContentsView:(TabContentsViewCocoa*)contentsView
                  dropData:(const WebDropData*)dropData
                     image:(NSImage*)image
                    offset:(NSPoint)offset
                pasteboard:(NSPasteboard*)pboard
         dragOperationMask:(NSDragOperation)dragOperationMask;

- (NSDragOperation)draggingSourceOperationMaskForLocal:(BOOL)isLocal;

- (void)lazyWriteToPasteboard:(NSPasteboard*)pboard
                      forType:(NSString*)type;

- (void)startDrag;

- (void)endDragAt:(NSPoint)screenPoint
        operation:(NSDragOperation)operation;

- (void)moveDragTo:(NSPoint)screenPoint;

- (NSString*)dragPromisedFileTo:(NSString*)path;

@end
