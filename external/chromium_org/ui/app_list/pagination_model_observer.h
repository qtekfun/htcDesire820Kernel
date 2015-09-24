// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_PAGINATION_MODEL_OBSERVER_H_
#define UI_APP_LIST_PAGINATION_MODEL_OBSERVER_H_

#include "ui/app_list/app_list_export.h"

namespace app_list {

class APP_LIST_EXPORT PaginationModelObserver {
 public:
  
  virtual void TotalPagesChanged() = 0;

  
  virtual void SelectedPageChanged(int old_selected, int new_selected) = 0;

  
  virtual void TransitionStarted() = 0;

  
  virtual void TransitionChanged() = 0;

 protected:
  virtual ~PaginationModelObserver() {}
};

}  

#endif  
