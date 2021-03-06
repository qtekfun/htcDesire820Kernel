// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_COCOA_APPS_SEARCH_RESULTS_CONTROLLER_H_
#define UI_APP_LIST_COCOA_APPS_SEARCH_RESULTS_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#include "ui/app_list/app_list_export.h"
#include "ui/app_list/app_list_model.h"
#import "ui/base/cocoa/tracking_area.h"

namespace app_list {
class AppsSearchResultsModelBridge;
class SearchResult;
}

@class AppsSearchResultsCell;

@protocol AppsSearchResultsDelegate<NSObject>

- (app_list::AppListModel*)appListModel;
- (void)openResult:(app_list::SearchResult*)result;
- (void)redoSearch;

@end

APP_LIST_EXPORT
@interface AppsSearchResultsController
    : NSViewController<NSTableViewDelegate, NSTableViewDataSource> {
 @private
  base::scoped_nsobject<NSTableView> tableView_;
  ui::ScopedCrTrackingArea trackingArea_;
  NSPoint lastMouseDownInView_;
  NSInteger hoveredRowIndex_;
  scoped_ptr<app_list::AppsSearchResultsModelBridge> bridge_;
  NSObject<AppsSearchResultsDelegate>* delegate_;  
}

@property(assign, nonatomic) NSObject<AppsSearchResultsDelegate>* delegate;
@property(readonly, nonatomic) app_list::AppListModel::SearchResults* results;
@property(readonly, nonatomic) NSTableView* tableView;

- (id)initWithAppsSearchResultsFrameSize:(NSSize)size;

- (BOOL)handleCommandBySelector:(SEL)command;

@end

#endif  
