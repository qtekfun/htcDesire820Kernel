// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_COREWM_SHADOW_TYPES_H_
#define UI_VIEWS_COREWM_SHADOW_TYPES_H_

#include "ui/aura/window.h"
#include "ui/views/views_export.h"

namespace aura {
class Window;
}

namespace views {
namespace corewm {

enum ShadowType {
  
  SHADOW_TYPE_NONE = 0,
  SHADOW_TYPE_RECTANGULAR,
};

VIEWS_EXPORT void SetShadowType(aura::Window* window, ShadowType shadow_type);
VIEWS_EXPORT ShadowType GetShadowType(aura::Window* window);

extern const aura::WindowProperty<ShadowType>* const kShadowTypeKey;

}  
}  

#endif  
