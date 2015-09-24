// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_SEPARATOR_H_
#define UI_VIEWS_CONTROLS_SEPARATOR_H_

#include <string>

#include "ui/views/view.h"

namespace views {


class VIEWS_EXPORT Separator : public View {
 public:
  enum Orientation {
    HORIZONTAL,
    VERTICAL
  };

  
  static const char kViewClassName[];

  explicit Separator(Orientation orientation);
  virtual ~Separator();

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;
  virtual void Paint(gfx::Canvas* canvas) OVERRIDE;
  virtual const char* GetClassName() const OVERRIDE;

 private:
  const Orientation orientation_;

  DISALLOW_COPY_AND_ASSIGN(Separator);
};

}  

#endif  
