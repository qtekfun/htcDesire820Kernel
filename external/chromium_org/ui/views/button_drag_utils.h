// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_BUTTON_DRAG_UTILS_H_
#define UI_VIEWS_BUTTON_DRAG_UTILS_H_

#include "base/strings/string16.h"
#include "ui/views/views_export.h"

class GURL;

namespace gfx {
class ImageSkia;
}

namespace ui {
class OSExchangeData;
}

namespace views {
class Widget;
}

namespace button_drag_utils {

VIEWS_EXPORT void SetURLAndDragImage(const GURL& url,
                                     const string16& title,
                                     const gfx::ImageSkia& icon,
                                     ui::OSExchangeData* data,
                                     views::Widget* widget);

}  

#endif  
