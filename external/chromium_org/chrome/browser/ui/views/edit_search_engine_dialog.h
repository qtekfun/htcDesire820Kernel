// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_UI_VIEWS_EDIT_SEARCH_ENGINE_DIALOG_H_
#define CHROME_BROWSER_UI_VIEWS_EDIT_SEARCH_ENGINE_DIALOG_H_

#include "ui/views/controls/textfield/textfield_controller.h"
#include "ui/views/window/dialog_delegate.h"

namespace views {
class Label;
class ImageView;
}

class EditSearchEngineController;
class EditSearchEngineControllerDelegate;
class Profile;
class TemplateURL;

class EditSearchEngineDialog : public views::TextfieldController,
                               public views::DialogDelegateView {
 public:
  
  EditSearchEngineDialog(TemplateURL* template_url,
                         EditSearchEngineControllerDelegate* delegate,
                         Profile* profile);
  virtual ~EditSearchEngineDialog();

  
  static void Show(gfx::NativeWindow parent,
                   TemplateURL* template_url,
                   EditSearchEngineControllerDelegate* delegate,
                   Profile* profile);

  
  virtual ui::ModalType GetModalType() const OVERRIDE;
  virtual base::string16 GetWindowTitle() const OVERRIDE;
  virtual bool IsDialogButtonEnabled(ui::DialogButton button) const OVERRIDE;
  virtual bool Cancel() OVERRIDE;
  virtual bool Accept() OVERRIDE;

  
  
  
  virtual void ContentsChanged(views::Textfield* sender,
                               const base::string16& new_contents) OVERRIDE;
  virtual bool HandleKeyEvent(views::Textfield* sender,
                              const ui::KeyEvent& key_event) OVERRIDE;
 private:
  void Init();

  
  views::Label* CreateLabel(int message_id);

  
  
  views::Textfield* CreateTextfield(const base::string16& text, bool lowercase);

  
  void UpdateImageViews();

  
  
  
  void UpdateImageView(views::ImageView* image_view,
                       bool is_valid,
                       int invalid_message_id);

  
  views::View* view_;

  
  views::Textfield* title_tf_;
  views::Textfield* keyword_tf_;
  views::Textfield* url_tf_;

  
  views::ImageView* title_iv_;
  views::ImageView* keyword_iv_;
  views::ImageView* url_iv_;

  scoped_ptr<EditSearchEngineController> controller_;

  DISALLOW_COPY_AND_ASSIGN(EditSearchEngineDialog);
};

#endif  
