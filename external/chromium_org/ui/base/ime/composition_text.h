// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_IME_COMPOSITION_TEXT_H_
#define UI_BASE_IME_COMPOSITION_TEXT_H_

#include "base/strings/string16.h"
#include "ui/base/ime/composition_underline.h"
#include "ui/base/ui_export.h"
#include "ui/gfx/range/range.h"

namespace ui {

struct UI_EXPORT CompositionText {
  CompositionText();
  ~CompositionText();

  bool operator==(const CompositionText& rhs) const {
    if ((this->text != rhs.text) ||
        (this->selection != rhs.selection) ||
        (this->underlines.size() != rhs.underlines.size()))
      return false;
    for (size_t i = 0; i < this->underlines.size(); ++i) {
      if (this->underlines[i] != rhs.underlines[i])
        return false;
    }
    return true;
  }

  bool operator!=(const CompositionText& rhs) const {
    return !(*this == rhs);
  }

  void Clear();

  
  base::string16 text;

  
  
  
  CompositionUnderlines underlines;

  
  
  
  
  gfx::Range selection;
};

}  

#endif  
