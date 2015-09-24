// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_URL_DROP_TARGET_H_
#define CHROME_BROWSER_UI_COCOA_URL_DROP_TARGET_H_

#import <Cocoa/Cocoa.h>

@protocol URLDropTarget;
@protocol URLDropTargetController;

@interface URLDropTargetHandler : NSObject {
 @private
  NSView<URLDropTarget>* view_;  
}

+ (NSArray*)handledDragTypes;

- (id)initWithView:(NSView<URLDropTarget>*)view;

- (NSDragOperation)draggingEntered:(id<NSDraggingInfo>)sender;
- (NSDragOperation)draggingUpdated:(id<NSDraggingInfo>)sender;
- (void)draggingExited:(id<NSDraggingInfo>)sender;
- (BOOL)performDragOperation:(id<NSDraggingInfo>)sender;

@end  

@protocol URLDropTarget

- (id<URLDropTargetController>)urlDropController;

- (NSDragOperation)draggingEntered:(id<NSDraggingInfo>)sender;
- (NSDragOperation)draggingUpdated:(id<NSDraggingInfo>)sender;
- (void)draggingExited:(id<NSDraggingInfo>)sender;
- (BOOL)performDragOperation:(id<NSDraggingInfo>)sender;

@end  

@protocol URLDropTargetController

- (BOOL)isUnsupportedDropData:(id<NSDraggingInfo>)info;

- (void)dropURLs:(NSArray*)urls inView:(NSView*)view at:(NSPoint)point;

- (void)dropText:(NSString*)text inView:(NSView*)view at:(NSPoint)point;

- (void)indicateDropURLsInView:(NSView*)view at:(NSPoint)point;

- (void)hideDropURLsIndicatorInView:(NSView*)view;

@end  

#endif  
