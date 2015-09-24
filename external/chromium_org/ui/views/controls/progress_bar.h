// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_PROGRESS_BAR_H_
#define UI_VIEWS_CONTROLS_PROGRESS_BAR_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/views/view.h"

namespace views {

class VIEWS_EXPORT ProgressBar : public View {
 public:
  
  ProgressBar();
  virtual ~ProgressBar();

  double current_value() const { return current_value_; }

  
  
  double GetNormalizedValue() const;

  
  
  void SetDisplayRange(double min_display_value, double max_display_value);

  
  
  void SetValue(double value);

  
  
  
  void SetTooltipText(const string16& tooltip_text);

  
  virtual bool GetTooltipText(const gfx::Point& p,
                              string16* tooltip) const OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

 private:
  static const char kViewClassName[];

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual const char* GetClassName() const OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;

  
  double min_display_value_;
  double max_display_value_;

  
  double current_value_;

  
  string16 tooltip_text_;

  DISALLOW_COPY_AND_ASSIGN(ProgressBar);
};

}  

#endif  
