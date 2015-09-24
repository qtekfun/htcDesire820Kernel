// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "base/files/file_path.h"
#include "base/mac/scoped_cftyperef.h"
#include "base/mac/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#include "content/common/content_export.h"
#include "url/gurl.h"

namespace content {
class WebContentsImpl;
struct DropData;
}

CONTENT_EXPORT
@interface WebDragSource : NSObject {
 @private
  
  content::WebContentsImpl* contents_;

  
  NSView* contentsView_;

  
  scoped_ptr<content::DropData> dropData_;

  
  base::scoped_nsobject<NSImage> dragImage_;

  
  NSPoint imageOffset_;

  
  base::scoped_nsobject<NSPasteboard> pasteboard_;

  
  NSDragOperation dragOperationMask_;

  
  base::FilePath downloadFileName_;

  
  GURL downloadURL_;

  
  base::ScopedCFTypeRef<CFStringRef> fileUTI_;
}

- (id)initWithContents:(content::WebContentsImpl*)contents
                  view:(NSView*)contentsView
              dropData:(const content::DropData*)dropData
                 image:(NSImage*)image
                offset:(NSPoint)offset
            pasteboard:(NSPasteboard*)pboard
     dragOperationMask:(NSDragOperation)dragOperationMask;

- (void)clearWebContentsView;

- (NSDragOperation)draggingSourceOperationMaskForLocal:(BOOL)isLocal;

- (void)lazyWriteToPasteboard:(NSPasteboard*)pboard
                      forType:(NSString*)type;

- (void)startDrag;

- (void)endDragAt:(NSPoint)screenPoint
        operation:(NSDragOperation)operation;

- (void)moveDragTo:(NSPoint)screenPoint;

- (NSString*)dragPromisedFileTo:(NSString*)path;

@end
