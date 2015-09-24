// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_CLIENT_AURA_CONSTANTS_H_
#define UI_AURA_CLIENT_AURA_CONSTANTS_H_

#include "ui/aura/aura_export.h"
#include "ui/aura/window.h"
#include "ui/base/ui_base_types.h"

namespace ui {
class InputMethod;
}

namespace aura {
namespace client {


AURA_EXPORT extern const WindowProperty<bool>* const kAlwaysOnTopKey;

AURA_EXPORT extern const WindowProperty<bool>* const kAnimationsDisabledKey;

AURA_EXPORT extern const WindowProperty<bool>* const kCanMaximizeKey;

AURA_EXPORT extern const WindowProperty<bool>* const kCanResizeKey;

AURA_EXPORT extern const WindowProperty<bool>* const kConstrainedWindowKey;

AURA_EXPORT extern const aura::WindowProperty<bool>* const kDrawAttentionKey;

AURA_EXPORT extern const WindowProperty<ui::ModalType>* const kModalKey;

AURA_EXPORT extern const WindowProperty<gfx::Rect*>* const kRestoreBoundsKey;

AURA_EXPORT extern const WindowProperty<ui::WindowShowState>* const
    kRestoreShowStateKey;

AURA_EXPORT extern const WindowProperty<ui::InputMethod*>* const
    kRootWindowInputMethodKey;

AURA_EXPORT extern const WindowProperty<ui::WindowShowState>* const
    kShowStateKey;


}  
}  

#endif  
