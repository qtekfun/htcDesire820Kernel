// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "content/common/content_export.h"
#include "content/public/common/drop_data.h"

namespace content {
class RenderViewHost;
class WebContentsImpl;
class WebDragDestDelegate;
}

typedef content::RenderViewHost* RenderViewHostIdentifier;

CONTENT_EXPORT
@interface WebDragDest : NSObject {
 @private
  
  content::WebContentsImpl* webContents_;

  
  content::WebDragDestDelegate* delegate_;

  
  
  NSDragOperation currentOperation_;

  
  
  RenderViewHostIdentifier currentRVH_;

  
  scoped_ptr<content::DropData> dropData_;

  
  bool canceled_;
}

- (id)initWithWebContentsImpl:(content::WebContentsImpl*)contents;

- (content::DropData*)currentDropData;

- (void)setDragDelegate:(content::WebDragDestDelegate*)delegate;

- (void)setCurrentOperation:(NSDragOperation)operation;

- (NSDragOperation)draggingEntered:(id<NSDraggingInfo>)info
                              view:(NSView*)view;
- (void)draggingExited:(id<NSDraggingInfo>)info;
- (NSDragOperation)draggingUpdated:(id<NSDraggingInfo>)info
                              view:(NSView*)view;
- (BOOL)performDragOperation:(id<NSDraggingInfo>)info
                              view:(NSView*)view;

@end

@interface WebDragDest(Testing)
- (void)populateDropData:(content::DropData*)data
             fromPasteboard:(NSPasteboard*)pboard;
- (NSPoint)flipWindowPointToView:(const NSPoint&)windowPoint
                            view:(NSView*)view;
- (NSPoint)flipWindowPointToScreen:(const NSPoint&)windowPoint
                              view:(NSView*)view;
@end
