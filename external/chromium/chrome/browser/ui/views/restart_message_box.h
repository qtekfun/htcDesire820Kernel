// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_RESTART_MESSAGE_BOX_H_
#define CHROME_BROWSER_UI_VIEWS_RESTART_MESSAGE_BOX_H_
#pragma once

#include "base/basictypes.h"
#include "ui/gfx/native_widget_types.h"
#include "views/window/dialog_delegate.h"

namespace views {
class MessageBoxView;
}

class RestartMessageBox : public views::DialogDelegate {
 public:
  
  static void ShowMessageBox(gfx::NativeWindow parent_window);

 protected:
  
  virtual int GetDialogButtons() const;
  virtual std::wstring GetDialogButtonLabel(
      MessageBoxFlags::DialogButton button) const;
  virtual std::wstring GetWindowTitle() const;

  
  virtual void DeleteDelegate();
  virtual bool IsModal() const;
  virtual views::View* GetContentsView();

 private:
  explicit RestartMessageBox(gfx::NativeWindow parent_window);
  virtual ~RestartMessageBox();

  views::MessageBoxView* message_box_view_;

  DISALLOW_COPY_AND_ASSIGN(RestartMessageBox);
};

#endif  
