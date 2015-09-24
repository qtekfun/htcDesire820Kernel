// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_COLOR_CHOOSER_DIALOG_H_
#define CHROME_BROWSER_UI_VIEWS_COLOR_CHOOSER_DIALOG_H_

#include "base/memory/ref_counted.h"
#include "chrome/browser/ui/views/color_chooser_dialog.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/shell_dialogs/base_shell_dialog.h"
#include "ui/shell_dialogs/base_shell_dialog_win.h"

namespace views {
class ColorChooserListener;
}

class ColorChooserDialog
    : public base::RefCountedThreadSafe<ColorChooserDialog>,
      public ui::BaseShellDialog,
      public ui::BaseShellDialogImpl {
 public:
  ColorChooserDialog(views::ColorChooserListener* listener,
                     SkColor initial_color,
                     gfx::NativeWindow owning_window);
  virtual ~ColorChooserDialog();

  
  virtual bool IsRunning(gfx::NativeWindow owning_window) const OVERRIDE;
  virtual void ListenerDestroyed() OVERRIDE;

 private:
  struct ExecuteOpenParams {
    ExecuteOpenParams(SkColor color, RunState run_state, HWND owner);
    SkColor color;
    RunState run_state;
    HWND owner;
  };

  
  
  
  void ExecuteOpen(const ExecuteOpenParams& params);

  
  
  
  
  
  void DidCloseDialog(bool chose_color, SkColor color, RunState run_state);

  
  void CopyCustomColors(COLORREF*, COLORREF*);

  
  
  static COLORREF g_custom_colors[16];

  
  
  
  
  COLORREF custom_colors_[16];

  
  
  
  views::ColorChooserListener* listener_;

  DISALLOW_COPY_AND_ASSIGN(ColorChooserDialog);
};

#endif  
