// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_UNINSTALL_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_UNINSTALL_VIEW_H_

#include <map>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/strings/string16.h"
#include "ui/base/models/combobox_model.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/window/dialog_delegate.h"

namespace views {
class Checkbox;
class Combobox;
class Label;
}

class UninstallView : public views::ButtonListener,
                      public views::DialogDelegateView,
                      public ui::ComboboxModel {
 public:
  explicit UninstallView(int* user_selection,
                         const base::Closure& quit_closure,
                         bool show_delete_profile);
  virtual ~UninstallView();

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual bool Accept() OVERRIDE;
  virtual bool Cancel() OVERRIDE;
  virtual base::string16 GetDialogButtonLabel(
      ui::DialogButton button) const OVERRIDE;

  
  virtual base::string16 GetWindowTitle() const OVERRIDE;

  
  virtual int GetItemCount() const OVERRIDE;
  virtual base::string16 GetItemAt(int index) OVERRIDE;

 private:
  typedef std::map<base::string16, base::string16> BrowsersMap;

  
  void SetupControls();

  views::Label* confirm_label_;
  bool show_delete_profile_;
  views::Checkbox* delete_profile_;
  views::Checkbox* change_default_browser_;
  views::Combobox* browsers_combo_;
  scoped_ptr<BrowsersMap> browsers_;
  int& user_selection_;
  base::Closure quit_closure_;

  DISALLOW_COPY_AND_ASSIGN(UninstallView);
};

#endif  