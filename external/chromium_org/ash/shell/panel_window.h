// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SHELL_PANEL_WINDOW_H_
#define ASH_SHELL_PANEL_WINDOW_H_

#include "base/basictypes.h"
#include "ui/aura/aura_export.h"
#include "ui/views/widget/widget.h"
#include "ui/views/widget/widget_delegate.h"

namespace ash {

class PanelFrameView;

class PanelWindow : public views::WidgetDelegateView {
 public:
  explicit PanelWindow(const std::string& name);
  virtual ~PanelWindow();

  
  views::Widget* CreateWidget();

  const std::string& name() { return name_; }
  views::Widget::InitParams& params() { return params_; }

  
  static views::Widget* CreatePanelWindow(const gfx::Rect& rect);

 private:
  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;

  
  virtual base::string16 GetWindowTitle() const OVERRIDE;
  virtual View* GetContentsView() OVERRIDE;
  virtual bool CanResize() const OVERRIDE;
  virtual bool CanMaximize() const OVERRIDE;
  virtual views::NonClientFrameView* CreateNonClientFrameView(
      views::Widget* widget) OVERRIDE;

  std::string name_;
  views::Widget::InitParams params_;

  DISALLOW_COPY_AND_ASSIGN(PanelWindow);
};

}  

#endif  
