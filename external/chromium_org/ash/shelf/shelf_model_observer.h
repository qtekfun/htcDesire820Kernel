// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SHELF_SHELF_MODEL_OBSERVER_H_
#define ASH_SHELF_SHELF_MODEL_OBSERVER_H_

#include "ash/ash_export.h"
#include "ash/launcher/launcher_types.h"

namespace ash {

struct LauncherItem;

class ASH_EXPORT ShelfModelObserver {
 public:
  
  virtual void ShelfItemAdded(int index) = 0;

  
  
  virtual void ShelfItemRemoved(int index, LauncherID id) = 0;

  
  
  virtual void ShelfItemMoved(int start_index, int target_index) = 0;

  
  
  virtual void ShelfItemChanged(int index, const LauncherItem& old_item) = 0;

  
  virtual void ShelfStatusChanged() = 0;

 protected:
  virtual ~ShelfModelObserver() {}
};

}  

#endif  
