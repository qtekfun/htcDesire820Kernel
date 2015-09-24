// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_NATIVE_THEME_DELEGATE_H_
#define UI_VIEWS_NATIVE_THEME_DELEGATE_H_

#include "ui/gfx/rect.h"
#include "ui/native_theme/native_theme.h"
#include "ui/views/views_export.h"

namespace views {

class VIEWS_EXPORT NativeThemeDelegate {
 public:
  virtual ~NativeThemeDelegate() {}

  
  virtual ui::NativeTheme::Part GetThemePart() const = 0;

  
  virtual gfx::Rect GetThemePaintRect() const = 0;

  
  virtual ui::NativeTheme::State GetThemeState(
      ui::NativeTheme::ExtraParams* params) const = 0;

  
  
  virtual const gfx::Animation* GetThemeAnimation() const = 0;

  
  virtual ui::NativeTheme::State GetBackgroundThemeState(
      ui::NativeTheme::ExtraParams* params) const = 0;

  
  
  
  virtual ui::NativeTheme::State GetForegroundThemeState(
      ui::NativeTheme::ExtraParams* params) const = 0;
};

}  

#endif  
