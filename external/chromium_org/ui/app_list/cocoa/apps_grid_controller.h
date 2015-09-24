// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_COCOA_APPS_GRID_CONTROLLER_H_
#define UI_APP_LIST_COCOA_APPS_GRID_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#include "ui/app_list/app_list_export.h"
#import "ui/app_list/cocoa/app_list_pager_view.h"
#import "ui/app_list/cocoa/scroll_view_with_no_scrollbars.h"

namespace app_list {
class AppListModel;
class AppListViewDelegate;
class AppsGridDelegateBridge;
}

@class AppsGridViewItem;
@protocol AppsPaginationModelObserver;
@class AppsCollectionViewDragManager;

APP_LIST_EXPORT
@interface AppsGridController : NSViewController<GestureScrollDelegate,
                                                 AppListPagerDelegate,
                                                 NSCollectionViewDelegate> {
 @private
  app_list::AppListViewDelegate* delegate_;  
  scoped_ptr<app_list::AppsGridDelegateBridge> bridge_;

  base::scoped_nsobject<AppsCollectionViewDragManager> dragManager_;
  base::scoped_nsobject<NSMutableArray> pages_;
  base::scoped_nsobject<NSMutableArray> items_;
  base::scoped_nsobject<NSTimer> scrollWhileDraggingTimer_;

  id<AppsPaginationModelObserver> paginationObserver_;

  
  size_t visiblePage_;
  
  size_t targetScrollPage_;
  
  size_t scheduledScrollPage_;

  
  BOOL animatingScroll_;
}

@property(assign, nonatomic) id<AppsPaginationModelObserver> paginationObserver;

+ (void)setScrollAnimationDuration:(NSTimeInterval)duration;

+ (CGFloat)scrollerPadding;

- (NSCollectionView*)collectionViewAtPageIndex:(size_t)pageIndex;
- (size_t)pageIndexForCollectionView:(NSCollectionView*)page;

- (AppsGridViewItem*)itemAtIndex:(size_t)itemIndex;

- (app_list::AppListModel*)model;

- (void)setDelegate:(app_list::AppListViewDelegate*)newDelegate;

- (size_t)visiblePage;

- (void)activateSelection;

- (size_t)pageCount;

- (size_t)itemCount;

- (void)scrollToPage:(size_t)pageIndex;

- (void)maybeChangePageForPoint:(NSPoint)locationInWindow;

- (void)cancelScrollTimer;

- (void)moveItemInView:(size_t)fromIndex
           toItemIndex:(size_t)toIndex;

- (void)moveItemWithIndex:(size_t)itemIndex
             toModelIndex:(size_t)modelIndex;

- (NSUInteger)selectedItemIndex;

- (void)selectItemAtIndex:(NSUInteger)index;

- (BOOL)handleCommandBySelector:(SEL)command;

@end

@interface AppsGridController(TestingAPI)

- (AppsCollectionViewDragManager*)dragManager;
- (size_t)scheduledScrollPage;

@end

#endif  
