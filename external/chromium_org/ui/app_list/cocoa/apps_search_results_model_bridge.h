// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_COCOA_APPS_SEARCH_RESULTS_MODEL_BRIDGE_H_
#define UI_APP_LIST_COCOA_APPS_SEARCH_RESULTS_MODEL_BRIDGE_H_

#include "base/mac/scoped_nsobject.h"
#include "base/memory/scoped_vector.h"
#include "ui/base/models/list_model_observer.h"

@class NSMenu;
@class AppsSearchResultsController;

namespace app_list {

class AppsSearchResultsModelBridge : public ui::ListModelObserver {
 public:
  explicit AppsSearchResultsModelBridge(
      AppsSearchResultsController* results_controller);
  virtual ~AppsSearchResultsModelBridge();

  
  
  NSMenu* MenuForItem(size_t index);

 private:
  
  class ItemObserver;

  void UpdateItemObservers();
  void ReloadDataForItems(size_t start, size_t count) const;

  
  virtual void ListItemsAdded(size_t start, size_t count) OVERRIDE;
  virtual void ListItemsRemoved(size_t start, size_t count) OVERRIDE;
  virtual void ListItemMoved(size_t index, size_t target_index) OVERRIDE;
  virtual void ListItemsChanged(size_t start, size_t count) OVERRIDE;

  AppsSearchResultsController* parent_;  
  ScopedVector<ItemObserver> item_observers_;

  DISALLOW_COPY_AND_ASSIGN(AppsSearchResultsModelBridge);
};

}  

#endif  
