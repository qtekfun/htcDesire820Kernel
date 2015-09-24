// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_COCOA_APPS_COLLECTION_VIEW_DRAG_MANAGER_H_
#define UI_APP_LIST_COCOA_APPS_COLLECTION_VIEW_DRAG_MANAGER_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#include "ui/app_list/app_list_export.h"

@class AppsGridController;
@class ItemDragController;

APP_LIST_EXPORT
@interface AppsCollectionViewDragManager : NSObject {
 @private
  base::scoped_nsobject<ItemDragController> itemDragController_;
  AppsGridController* gridController_;  

  NSSize cellSize_;
  size_t rows_;
  size_t columns_;

  
  size_t itemDragIndex_;

  
  
  size_t itemHitIndex_;

  
  NSPoint mouseDownLocation_;

  
  BOOL dragging_;
}

- (id)initWithCellSize:(NSSize)cellSize
                  rows:(size_t)rows
               columns:(size_t)columns
        gridController:(AppsGridController*)gridController;

- (NSCollectionView*)makePageWithFrame:(NSRect)pageFrame;

- (void)cancelDrag;

@end

@interface AppsCollectionViewDragManager (TestingAPI)

- (void)onMouseDownInPage:(NSCollectionView*)page
                withEvent:(NSEvent*)theEvent;
- (void)onMouseDragged:(NSEvent*)theEvent;
- (void)onMouseUp:(NSEvent*)theEvent;

@end

#endif  
