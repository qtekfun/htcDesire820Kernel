// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SHELF_SHELF_ICON_OBSERVER_H_
#define ASH_SHELF_SHELF_ICON_OBSERVER_H_

#include "ash/ash_export.h"

namespace ash {

class ASH_EXPORT ShelfIconObserver {
 public:
  
  virtual void OnShelfIconPositionsChanged() = 0;

 protected:
  virtual ~ShelfIconObserver() {}
};

}  

#endif  
