// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_TOOLTIP_MANAGER_H_
#define UI_VIEWS_WIDGET_TOOLTIP_MANAGER_H_

#include <string>

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/views/views_export.h"

namespace gfx {
class Display;
class FontList;
}  

namespace views {

class View;

class VIEWS_EXPORT TooltipManager {
 public:
  
  
  
  
  
  
  
  static const char kGroupingPropertyKey[];

  TooltipManager() {}
  virtual ~TooltipManager() {}

  
  
  static int GetTooltipHeight();

  
  
  
  static int GetMaxWidth(int x, int y, gfx::NativeView context);

  
  static int GetMaxWidth(const gfx::Display& display);

  
  
  static void TrimTooltipText(base::string16* text);

  
  virtual const gfx::FontList& GetFontList() const = 0;

  
  virtual void UpdateTooltip() = 0;

  
  virtual void TooltipTextChanged(View* view) = 0;
};

}  

#endif  
