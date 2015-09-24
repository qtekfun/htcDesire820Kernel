// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_UI_ECHO_DIALOG_VIEW_H_
#define CHROME_BROWSER_CHROMEOS_UI_ECHO_DIALOG_VIEW_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/views/controls/styled_label_listener.h"
#include "ui/views/window/dialog_delegate.h"

namespace views {
class StyledLabel;
}

namespace chromeos {

class EchoDialogListener;

class EchoDialogView : public views::DialogDelegateView,
                       public views::StyledLabelListener {
 public:
  explicit EchoDialogView(EchoDialogListener* listener);
  virtual ~EchoDialogView();

  
  
  
  
  
  
  void InitForEnabledEcho(const base::string16& service_name, const base::string16& origin);

  
  
  
  
  void InitForDisabledEcho();

  
  void Show(gfx::NativeWindow parent);

 private:
  friend class ExtensionEchoPrivateApiTest;

  
  virtual int GetDialogButtons() const OVERRIDE;
  virtual int GetDefaultDialogButton() const OVERRIDE;
  virtual base::string16 GetDialogButtonLabel(ui::DialogButton button) const OVERRIDE;
  virtual bool Cancel() OVERRIDE;
  virtual bool Accept() OVERRIDE;

  
  virtual ui::ModalType GetModalType() const OVERRIDE;
  virtual bool ShouldShowWindowTitle() const OVERRIDE;
  virtual bool ShouldShowWindowIcon() const OVERRIDE;

  
  virtual void StyledLabelLinkClicked(const gfx::Range& range,
                                      int event_flags) OVERRIDE;

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;

  
  
  void SetLabelBorderAndBounds();

  views::StyledLabel* label_;
  EchoDialogListener* listener_;
  int ok_button_label_id_;
  int cancel_button_label_id_;

  DISALLOW_COPY_AND_ASSIGN(EchoDialogView);
};

}  

#endif  
