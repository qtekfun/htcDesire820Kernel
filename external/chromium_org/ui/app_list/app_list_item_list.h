// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_APP_LIST_ITEM_LIST_H_
#define UI_APP_LIST_APP_LIST_ITEM_LIST_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/observer_list.h"
#include "sync/api/string_ordinal.h"
#include "ui/app_list/app_list_export.h"
#include "ui/app_list/app_list_item_list_observer.h"

namespace app_list {

class AppListItemModel;

class APP_LIST_EXPORT AppListItemList {
 public:
  AppListItemList();
  virtual ~AppListItemList();

  void AddObserver(AppListItemListObserver* observer);
  void RemoveObserver(AppListItemListObserver* observer);

  
  AppListItemModel* FindItem(const std::string& id);

  
  
  
  bool FindItemIndex(const std::string& id, size_t* index);

  
  
  size_t AddItem(AppListItemModel* item);

  
  
  void InsertItemAt(AppListItemModel* item, size_t index);

  
  
  
  void DeleteItem(const std::string& id);

  
  
  
  
  void DeleteItemsByType(const char* type);

  
  
  scoped_ptr<AppListItemModel> RemoveItem(const std::string& id);

  
  
  scoped_ptr<AppListItemModel> RemoveItemAt(size_t index);

  
  
  void MoveItem(size_t from_index, size_t to_index);

  
  
  void SetItemPosition(AppListItemModel* item,
                       const syncer::StringOrdinal& new_position);

  AppListItemModel* item_at(size_t index) {
    DCHECK_LT(index, app_list_items_.size());
    return app_list_items_[index];
  }
  size_t item_count() const { return app_list_items_.size(); }

 private:
  
  void DeleteItemAt(size_t index);

  
  
  void EnsureValidItemPosition(AppListItemModel* item);

  
  
  size_t GetItemSortOrderIndex(const syncer::StringOrdinal& position,
                               const std::string& id);

  ScopedVector<AppListItemModel> app_list_items_;
  ObserverList<AppListItemListObserver> observers_;

  DISALLOW_COPY_AND_ASSIGN(AppListItemList);
};

}  

#endif  
