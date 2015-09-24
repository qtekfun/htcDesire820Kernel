// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_COCOA_APPS_GRID_VIEW_ITEM_H_
#define UI_APP_LIST_COCOA_APPS_GRID_VIEW_ITEM_H_

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_ptr.h"
#include "ui/app_list/app_list_export.h"
#import "ui/base/cocoa/tracking_area.h"

namespace app_list {
class AppListItemModel;
class ItemModelObserverBridge;
}

APP_LIST_EXPORT
@interface AppsGridViewItem : NSCollectionViewItem {
 @private
  scoped_ptr<app_list::ItemModelObserverBridge> observerBridge_;
  base::scoped_nsobject<NSProgressIndicator> progressIndicator_;

  
  ui::ScopedCrTrackingArea trackingArea_;
}

@property(readonly, nonatomic) NSProgressIndicator* progressIndicator;

- (id)initWithSize:(NSSize)tileSize;

- (void)setModel:(app_list::AppListItemModel*)itemModel;

- (app_list::AppListItemModel*)model;

- (NSButton*)button;

- (NSMenu*)contextMenu;

- (NSBitmapImageRep*)dragRepresentationForRestore:(BOOL)isRestore;

@end

#endif  
