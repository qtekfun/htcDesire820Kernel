// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_UPDATE_RECOMMENDED_MESSAGE_BOX_H_
#define CHROME_BROWSER_UI_VIEWS_UPDATE_RECOMMENDED_MESSAGE_BOX_H_
#pragma once

#include "base/basictypes.h"
#include "ui/gfx/native_widget_types.h"
#include "views/window/dialog_delegate.h"

namespace views {
class MessageBoxView;
}

class UpdateRecommendedMessageBox : public views::DialogDelegate {
 public:
  
  static void ShowMessageBox(gfx::NativeWindow parent_window);

  
  virtual bool Accept() OVERRIDE;

 protected:
  
  virtual int GetDialogButtons() const OVERRIDE;
  virtual std::wstring GetDialogButtonLabel(
      MessageBoxFlags::DialogButton button) const OVERRIDE;

  
  virtual bool ShouldShowWindowTitle() const OVERRIDE;
  virtual std::wstring GetWindowTitle() const OVERRIDE;
  virtual void DeleteDelegate() OVERRIDE;
  virtual bool IsModal() const OVERRIDE;
  virtual views::View* GetContentsView() OVERRIDE;

 private:
  explicit UpdateRecommendedMessageBox(gfx::NativeWindow parent_window);
  virtual ~UpdateRecommendedMessageBox();

  views::MessageBoxView* message_box_view_;

  DISALLOW_COPY_AND_ASSIGN(UpdateRecommendedMessageBox);
};

#endif  
