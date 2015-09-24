// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_GTK_EVENT_SYNTHESIS_GTK_
#define UI_BASE_GTK_EVENT_SYNTHESIS_GTK_

#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>
#include <vector>

#include "ui/base/ui_export.h"
#include "ui/events/keycodes/keyboard_codes.h"

namespace ui {

UI_EXPORT GdkEvent* SynthesizeKeyEvent(GdkWindow* event_window,
                                       bool press,
                                       guint gdk_key,
                                       guint state);

UI_EXPORT void SynthesizeKeyPressEvents(
    GdkWindow* window,
    KeyboardCode key,
    bool control, bool shift, bool alt,
    std::vector<GdkEvent*>* events);

}  

#endif  
