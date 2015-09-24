// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_APP_LIST_ITEM_MODEL_OBSERVER_H_
#define UI_APP_LIST_APP_LIST_ITEM_MODEL_OBSERVER_H_

#include "ui/app_list/app_list_export.h"

namespace app_list {

class APP_LIST_EXPORT AppListItemModelObserver {
 public:
  
  virtual void ItemIconChanged() = 0;

  
  virtual void ItemTitleChanged() = 0;

  
  virtual void ItemHighlightedChanged() = 0;

  
  virtual void ItemIsInstallingChanged() = 0;

  
  virtual void ItemPercentDownloadedChanged() = 0;

 protected:
  virtual ~AppListItemModelObserver() {}
};

}  

#endif  
