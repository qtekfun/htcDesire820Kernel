// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_VIEW_CONSTANTS_AURA_H_
#define UI_VIEWS_VIEW_CONSTANTS_AURA_H_

#include "ui/aura/window.h"
#include "ui/views/views_export.h"

namespace views {
class View;

VIEWS_EXPORT extern const aura::WindowProperty<View*>* const kHostViewKey;

}  

#endif  
