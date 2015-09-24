// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "base/string16.h"

class GURL;
class RenderViewHost;
class TabContents;
struct WebDropData;

typedef RenderViewHost* RenderViewHostIdentifier;


@interface WebDropTarget : NSObject {
 @private
  
  TabContents* tabContents_;

  
  
  NSDragOperation current_operation_;

  
  
  RenderViewHostIdentifier currentRVH_;
}

- (id)initWithTabContents:(TabContents*)contents;

- (void)setCurrentOperation: (NSDragOperation)operation;

- (NSDragOperation)draggingEntered:(id<NSDraggingInfo>)info
                              view:(NSView*)view;
- (void)draggingExited:(id<NSDraggingInfo>)info;
- (NSDragOperation)draggingUpdated:(id<NSDraggingInfo>)info
                              view:(NSView*)view;
- (BOOL)performDragOperation:(id<NSDraggingInfo>)info
                              view:(NSView*)view;

@end

@interface WebDropTarget(Testing)
- (BOOL)populateURL:(GURL*)url
    andTitle:(string16*)title
    fromPasteboard:(NSPasteboard*)pboard
    convertingFilenames:(BOOL)convertFilenames;
- (void)populateWebDropData:(WebDropData*)data
             fromPasteboard:(NSPasteboard*)pboard;
- (NSPoint)flipWindowPointToView:(const NSPoint&)windowPoint
                            view:(NSView*)view;
- (NSPoint)flipWindowPointToScreen:(const NSPoint&)windowPoint
                              view:(NSView*)view;
@end
