// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SHELL_TOPLEVEL_WINDOW_H_
#define ASH_SHELL_TOPLEVEL_WINDOW_H_

#include "ui/views/widget/widget_delegate.h"

namespace ash {
namespace shell {

class ToplevelWindow : public views::WidgetDelegateView {
 public:
  struct CreateParams {
    CreateParams();

    bool can_resize;
    bool can_maximize;
  };
  static views::Widget* CreateToplevelWindow(
      const CreateParams& params);

  
  
  static void ClearSavedStateForTest();

 private:
  explicit ToplevelWindow(const CreateParams& params);
  virtual ~ToplevelWindow();

  
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;

  
  virtual base::string16 GetWindowTitle() const OVERRIDE;
  virtual void SaveWindowPlacement(
      const gfx::Rect& bounds,
      ui::WindowShowState show_state) OVERRIDE;
  virtual bool GetSavedWindowPlacement(
      const views::Widget* widget,
      gfx::Rect* bounds,
      ui::WindowShowState* show_state) const OVERRIDE;
  virtual View* GetContentsView() OVERRIDE;
  virtual bool CanResize() const OVERRIDE;
  virtual bool CanMaximize() const OVERRIDE;

  const CreateParams params_;

  DISALLOW_COPY_AND_ASSIGN(ToplevelWindow);
};

}  
}  

#endif  
