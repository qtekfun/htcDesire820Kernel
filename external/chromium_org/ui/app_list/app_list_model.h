// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_APP_LIST_MODEL_H_
#define UI_APP_LIST_APP_LIST_MODEL_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "ui/app_list/app_list_export.h"
#include "ui/app_list/app_list_item_list.h"
#include "ui/base/models/list_model.h"

namespace app_list {

class AppListItemList;
class AppListItemModel;
class AppListModelObserver;
class SearchBoxModel;
class SearchResult;

class APP_LIST_EXPORT AppListModel {
 public:
  enum Status {
    STATUS_NORMAL,
    STATUS_SYNCING,  
  };

  typedef ui::ListModel<SearchResult> SearchResults;

  AppListModel();
  ~AppListModel();

  void AddObserver(AppListModelObserver* observer);
  void RemoveObserver(AppListModelObserver* observer);

  void SetStatus(Status status);

  AppListItemList* item_list() { return item_list_.get(); }
  SearchBoxModel* search_box() { return search_box_.get(); }
  SearchResults* results() { return results_.get(); }
  Status status() const { return status_; }

 private:
  scoped_ptr<AppListItemList> item_list_;
  scoped_ptr<SearchBoxModel> search_box_;
  scoped_ptr<SearchResults> results_;

  Status status_;
  ObserverList<AppListModelObserver> observers_;

  DISALLOW_COPY_AND_ASSIGN(AppListModel);
};

}  

#endif  
