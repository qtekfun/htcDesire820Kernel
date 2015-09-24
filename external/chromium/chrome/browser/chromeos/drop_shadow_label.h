// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DROP_SHADOW_LABEL_H_
#define CHROME_BROWSER_CHROMEOS_DROP_SHADOW_LABEL_H_
#pragma once

#include "ui/gfx/font.h"
#include "views/controls/label.h"

namespace chromeos {

class DropShadowLabel : public views::Label  {
 public:
  DropShadowLabel();

  
  
  
  
  
  
  void SetDropShadowSize(int size);

  
  int drop_shadow_size() const { return drop_shadow_size_; }

  
  virtual void PaintText(gfx::Canvas* canvas,
                         const std::wstring& text,
                         const gfx::Rect& text_bounds,
                         int flags);

 protected:
  virtual gfx::Size GetTextSize() const;

 private:
  void Init();

  int drop_shadow_size_;

  DISALLOW_COPY_AND_ASSIGN(DropShadowLabel);
};

}  

#endif  
