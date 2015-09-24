// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_TRAY_TRAY_UTILS_H_
#define ASH_SYSTEM_TRAY_TRAY_UTILS_H_

#include "ash/shelf/shelf_types.h"

namespace views {
class Label;
class View;
}

namespace ash {
namespace internal {

class TrayItemView;

void SetupLabelForTray(views::Label* label);

void SetTrayImageItemBorder(views::View* tray_view, ShelfAlignment alignment);
void SetTrayLabelItemBorder(TrayItemView* tray_view,
                            ShelfAlignment alignment);

}  
}  

#endif  
