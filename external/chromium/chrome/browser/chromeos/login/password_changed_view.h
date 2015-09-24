// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_PASSWORD_CHANGED_VIEW_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_PASSWORD_CHANGED_VIEW_H_
#pragma once

#include <string>

#include "views/controls/button/button.h"
#include "views/controls/textfield/textfield_controller.h"
#include "views/view.h"
#include "views/window/dialog_delegate.h"

namespace views {
class Button;
class Label;
class RadioButton;
class Textfield;
}  

namespace chromeos {

class PasswordChangedView : public views::View,
                            public views::DialogDelegate,
                            public views::ButtonListener,
                            public views::TextfieldController {
 public:
  
  class Delegate {
  public:
    virtual ~Delegate() {}

    
    virtual void RecoverEncryptedData(const std::string& old_password) = 0;

    
    virtual void ResyncEncryptedData() = 0;
  };

  PasswordChangedView(Delegate* delegate, bool full_sync_disabled);
  virtual ~PasswordChangedView() {}

  
  virtual bool Accept();
  virtual int GetDialogButtons() const;

  
  virtual View* GetInitiallyFocusedView();
  virtual bool IsModal() const { return true; }
  virtual views::View* GetContentsView() { return this; }

  
  virtual std::wstring GetWindowTitle() const;

  
  virtual void ButtonPressed(views::Button* sender,
                             const views::Event& event);

  
  virtual bool HandleKeyEvent(views::Textfield* sender,
                              const views::KeyEvent& keystroke) {
    return false;
  }
  virtual void ContentsChanged(views::Textfield* sender,
                               const string16& new_contents) {}

 protected:
  
  virtual gfx::Size GetPreferredSize();
  virtual void ViewHierarchyChanged(bool is_add,
                                    views::View* parent,
                                    views::View* child);

 private:
  
  bool ExitDialog();

  
  void Init();

  
  views::Label* title_label_;
  views::Label* description_label_;
  views::RadioButton* full_sync_radio_;
  views::RadioButton* delta_sync_radio_;
  views::Textfield* old_password_field_;

  
  Delegate* delegate_;

  
  bool full_sync_disabled_;

  DISALLOW_COPY_AND_ASSIGN(PasswordChangedView);
};

}  

#endif  
