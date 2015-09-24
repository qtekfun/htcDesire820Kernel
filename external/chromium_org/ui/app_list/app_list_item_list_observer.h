// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_APP_LIST_ITEM_LIST_OBSERVER_H_
#define UI_APP_LIST_APP_LIST_ITEM_LIST_OBSERVER_H_

#include "base/basictypes.h"

namespace app_list {

class AppListItemModel;

class APP_LIST_EXPORT AppListItemListObserver {
 public:
  
  virtual void OnListItemAdded(size_t index, AppListItemModel* item) {}

  
  
  virtual void OnListItemRemoved(size_t index, AppListItemModel* item) {}

  
  virtual void OnListItemMoved(size_t from_index,
                               size_t to_index,
                               AppListItemModel* item) {}

 protected:
  virtual ~AppListItemListObserver() {}
};

}  

#endif  
