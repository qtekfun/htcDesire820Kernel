// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_EVENT_UTILS_H_
#define UI_VIEWS_EVENT_UTILS_H_

#include "ui/gfx/native_widget_types.h"
#include "ui/views/views_export.h"

namespace ui {
class LocatedEvent;
}

namespace views {

VIEWS_EXPORT bool RepostLocatedEvent(gfx::NativeWindow window,
                                     const ui::LocatedEvent& event);

#if defined(OS_WIN) && defined(USE_AURA)
VIEWS_EXPORT bool RepostLocatedEventWin(HWND window,
                                        const ui::LocatedEvent& event);
#endif

}  

#endif  
