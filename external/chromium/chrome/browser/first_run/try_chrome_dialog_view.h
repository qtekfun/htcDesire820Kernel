// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_FIRST_RUN_TRY_CHROME_DIALOG_VIEW_H_
#define CHROME_BROWSER_FIRST_RUN_TRY_CHROME_DIALOG_VIEW_H_
#pragma once

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "views/controls/button/button.h"
#include "views/controls/link.h"
#include "ui/gfx/native_widget_types.h"

class ProcessSingleton;

namespace gfx {
class Rect;
}

namespace views {
class RadioButton;
class Widget;
}

class TryChromeDialogView : public views::ButtonListener,
                            public views::LinkController {
 public:
  enum Result {
    TRY_CHROME,          
    NOT_NOW,             
    UNINSTALL_CHROME,    
    DIALOG_ERROR,        
    COUNT
  };

  
  
  
  
  
  
  
  
  
  static Result Show(size_t version, ProcessSingleton* process_singleton);

 private:
  enum ButtonTags {
    BT_NONE,
    BT_CLOSE_BUTTON,
    BT_OK_BUTTON,
  };

  explicit TryChromeDialogView(size_t version);
  virtual ~TryChromeDialogView();

  Result ShowModal(ProcessSingleton* process_singleton);

  
  
  
  
  
  gfx::Rect ComputeWindowPosition(int width, int height, bool is_RTL);

  
  
  void SetToastRegion(gfx::NativeWindow window, int w, int h);

  
  
  
  virtual void ButtonPressed(views::Button* sender,
                             const views::Event& event) OVERRIDE;

  
  
  
  virtual void LinkActivated(views::Link* source, int event_flags) OVERRIDE;

  
  size_t version_;

  
  
  views::Widget* popup_;
  views::RadioButton* try_chrome_;
  views::RadioButton* kill_chrome_;
  Result result_;

  DISALLOW_COPY_AND_ASSIGN(TryChromeDialogView);
};

#endif  
