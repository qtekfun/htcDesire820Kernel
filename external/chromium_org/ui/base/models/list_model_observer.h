// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_MODELS_LIST_MODEL_OBSERVER_H_
#define UI_BASE_MODELS_LIST_MODEL_OBSERVER_H_

#include <stddef.h>

#include "ui/base/ui_export.h"

namespace ui {

class UI_EXPORT ListModelObserver {
 public:
  
  virtual void ListItemsAdded(size_t start, size_t count) = 0;

  
  
  virtual void ListItemsRemoved(size_t start, size_t count) = 0;

  
  
  virtual void ListItemMoved(size_t index, size_t target_index) = 0;

  
  virtual void ListItemsChanged(size_t start, size_t count) = 0;

 protected:
  virtual ~ListModelObserver() {}
};

}  

#endif  
