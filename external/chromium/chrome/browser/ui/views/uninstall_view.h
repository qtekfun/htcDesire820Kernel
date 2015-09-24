// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_UNINSTALL_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_UNINSTALL_VIEW_H_
#pragma once

#include "base/string16.h"
#include "ui/base/models/combobox_model.h"
#include "views/controls/combobox/combobox.h"
#include "views/window/dialog_delegate.h"

namespace views {
class Checkbox;
class Label;
}

class UninstallView : public views::View,
                      public views::ButtonListener,
                      public views::DialogDelegate,
                      public ui::ComboboxModel {
 public:
  explicit UninstallView(int& user_selection);
  virtual ~UninstallView();

  
  virtual bool Accept();
  virtual bool Cancel();
  virtual std::wstring GetDialogButtonLabel(
      MessageBoxFlags::DialogButton button) const;

  
  virtual void ButtonPressed(views::Button* sender, const views::Event& event);

  
  virtual std::wstring GetWindowTitle() const;
  virtual views::View* GetContentsView();

  
  virtual int GetItemCount();
  virtual string16 GetItemAt(int index);

 private:
  
  void SetupControls();

  views::Label* confirm_label_;
  views::Checkbox* delete_profile_;
  views::Checkbox* change_default_browser_;
  views::Combobox* browsers_combo_;
  typedef std::map<std::wstring, std::wstring> BrowsersMap;
  scoped_ptr<BrowsersMap> browsers_;
  int& user_selection_;

  DISALLOW_COPY_AND_ASSIGN(UninstallView);
};

#endif  
