// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SHELF_SHELF_NAVIGATOR_H_
#define ASH_SHELF_SHELF_NAVIGATOR_H_

#include "ash/ash_export.h"
#include "ash/launcher/launcher_types.h"

namespace ash {

class ShelfModel;

ASH_EXPORT int GetNextActivatedItemIndex(const ShelfModel& model,
                                         CycleDirection direction);

}  

#endif  
