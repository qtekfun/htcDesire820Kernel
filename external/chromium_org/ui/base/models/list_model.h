// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_MODELS_LIST_MODEL_H_
#define UI_BASE_MODELS_LIST_MODEL_H_

#include "base/basictypes.h"
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/observer_list.h"
#include "ui/base/models/list_model_observer.h"

namespace ui {

template <class ItemType>
class ListModel {
 public:
  ListModel() {}
  ~ListModel() {}

  
  void AddAt(size_t index, ItemType* item) {
    DCHECK_LE(index, item_count());
    items_.insert(items_.begin() + index, item);
    NotifyItemsAdded(index, 1);
  }

  
  void Add(ItemType* item) {
    AddAt(item_count(), item);
  }

  
  
  scoped_ptr<ItemType> RemoveAt(size_t index) {
    DCHECK_LT(index, item_count());
    ItemType* item = items_[index];
    items_.weak_erase(items_.begin() + index);
    NotifyItemsRemoved(index, 1);
    return make_scoped_ptr<ItemType>(item);
  }

  
  void RemoveAll() {
    size_t count = item_count();
    items_.weak_clear();
    NotifyItemsRemoved(0, count);
  }

  
  void DeleteAt(size_t index) {
    scoped_ptr<ItemType> item = RemoveAt(index);
    
  }

  
  void DeleteAll() {
    ScopedVector<ItemType> to_be_deleted(items_.Pass());
    NotifyItemsRemoved(0, to_be_deleted.size());
  }

  
  
  void Move(size_t index, size_t target_index) {
    DCHECK_LT(index, item_count());
    DCHECK_LT(target_index, item_count());

    if (index == target_index)
      return;

    ItemType* item = items_[index];
    items_.weak_erase(items_.begin() + index);
    items_.insert(items_.begin() + target_index, item);
    NotifyItemMoved(index, target_index);
  }

  void AddObserver(ListModelObserver* observer) {
    observers_.AddObserver(observer);
  }

  void RemoveObserver(ListModelObserver* observer) {
    observers_.RemoveObserver(observer);
  }

  void NotifyItemsAdded(size_t start, size_t count) {
    FOR_EACH_OBSERVER(ListModelObserver,
                      observers_,
                      ListItemsAdded(start, count));
  }

  void NotifyItemsRemoved(size_t start, size_t count) {
    FOR_EACH_OBSERVER(ListModelObserver,
                      observers_,
                      ListItemsRemoved(start, count));
  }

  void NotifyItemMoved(size_t index, size_t target_index) {
    FOR_EACH_OBSERVER(ListModelObserver,
                      observers_,
                      ListItemMoved(index, target_index));
  }

  void NotifyItemsChanged(size_t start, size_t count) {
    FOR_EACH_OBSERVER(ListModelObserver,
                      observers_,
                      ListItemsChanged(start, count));
  }

  size_t item_count() const { return items_.size(); }

  const ItemType* GetItemAt(size_t index) const {
    DCHECK_LT(index, item_count());
    return items_[index];
  }
  ItemType* GetItemAt(size_t index) {
    return const_cast<ItemType*>(
        const_cast<const ListModel<ItemType>*>(this)->GetItemAt(index));
  }

 private:
  ScopedVector<ItemType> items_;
  ObserverList<ListModelObserver> observers_;

  DISALLOW_COPY_AND_ASSIGN(ListModel<ItemType>);
};

}  

#endif  
