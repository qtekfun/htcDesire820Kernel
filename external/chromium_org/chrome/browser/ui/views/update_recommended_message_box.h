// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_UPDATE_RECOMMENDED_MESSAGE_BOX_H_
#define CHROME_BROWSER_UI_VIEWS_UPDATE_RECOMMENDED_MESSAGE_BOX_H_

#include "base/basictypes.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/views/window/dialog_delegate.h"

namespace views {
class MessageBoxView;
}

class UpdateRecommendedMessageBox : public views::DialogDelegate {
 public:
  static void Show(gfx::NativeWindow parent_window);

 private:
  UpdateRecommendedMessageBox();
  virtual ~UpdateRecommendedMessageBox();

  
  virtual bool Accept() OVERRIDE;
  virtual base::string16 GetDialogButtonLabel(
      ui::DialogButton button) const OVERRIDE;

  
  virtual bool ShouldShowWindowTitle() const OVERRIDE;
  virtual base::string16 GetWindowTitle() const OVERRIDE;
  virtual void DeleteDelegate() OVERRIDE;
  virtual ui::ModalType GetModalType() const OVERRIDE;
  virtual views::View* GetContentsView() OVERRIDE;
  virtual views::Widget* GetWidget() OVERRIDE;
  virtual const views::Widget* GetWidget() const OVERRIDE;

  views::MessageBoxView* message_box_view_;

  DISALLOW_COPY_AND_ASSIGN(UpdateRecommendedMessageBox);
};

#endif  
