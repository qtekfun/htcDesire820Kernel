// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_FIRST_RUN_TRY_CHROME_DIALOG_VIEW_H_
#define CHROME_BROWSER_FIRST_RUN_TRY_CHROME_DIALOG_VIEW_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/link_listener.h"

class ProcessSingleton;

namespace gfx {
class Rect;
}

namespace views {
class RadioButton;
class Checkbox;
class Widget;
}


class TryChromeDialogView : public views::ButtonListener,
                            public views::LinkListener {
 public:
  
  
  typedef base::Callback<void(gfx::NativeWindow active_dialog)>
      ActiveModalDialogListener;

  enum Result {
    TRY_CHROME,             
    TRY_CHROME_AS_DEFAULT,  
    NOT_NOW,                
    UNINSTALL_CHROME,       
    DIALOG_ERROR,           
    COUNT
  };

  
  
  
  
  
  
  
  
  
  static Result Show(size_t flavor, const ActiveModalDialogListener& listener);

 private:
  explicit TryChromeDialogView(size_t flavor);
  virtual ~TryChromeDialogView();

  Result ShowModal(const ActiveModalDialogListener& listener);

  
  
  
  
  
  gfx::Rect ComputeWindowPosition(int width, int height, bool is_RTL);

  
  
  void SetToastRegion(HWND window, int w, int h);

  
  
  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  
  
  virtual void LinkClicked(views::Link* source, int event_flags) OVERRIDE;

  
  size_t flavor_;

  
  
  views::Widget* popup_;
  views::RadioButton* try_chrome_;
  views::RadioButton* kill_chrome_;
  views::RadioButton* dont_try_chrome_;
  views::Checkbox* make_default_;
  Result result_;

  DISALLOW_COPY_AND_ASSIGN(TryChromeDialogView);
};

#endif  
