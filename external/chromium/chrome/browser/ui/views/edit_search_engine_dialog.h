// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_UI_VIEWS_EDIT_SEARCH_ENGINE_DIALOG_H_
#define CHROME_BROWSER_UI_VIEWS_EDIT_SEARCH_ENGINE_DIALOG_H_
#pragma once

#include <windows.h>

#include "views/controls/textfield/textfield_controller.h"
#include "views/window/dialog_delegate.h"

namespace views {
class Label;
class ImageView;
class Window;
}

class EditSearchEngineController;
class EditSearchEngineControllerDelegate;
class Profile;
class TemplateURL;
class TemplateURLModel;

class EditSearchEngineDialog : public views::View,
                               public views::TextfieldController,
                               public views::DialogDelegate {
 public:
  
  EditSearchEngineDialog(const TemplateURL* template_url,
                         EditSearchEngineControllerDelegate* delegate,
                         Profile* profile);
  virtual ~EditSearchEngineDialog() {}

  
  static void Show(gfx::NativeWindow parent,
                   const TemplateURL* template_url,
                   EditSearchEngineControllerDelegate* delegate,
                   Profile* profile);

  
  virtual bool IsModal() const;
  virtual std::wstring GetWindowTitle() const;
  virtual bool IsDialogButtonEnabled(
      MessageBoxFlags::DialogButton button) const;
  virtual bool Cancel();
  virtual bool Accept();
  virtual views::View* GetContentsView();

  
  
  
  virtual void ContentsChanged(views::Textfield* sender,
                               const std::wstring& new_contents);
  virtual bool HandleKeyEvent(views::Textfield* sender,
                              const views::KeyEvent& key_event);

 private:
  void Init();

  
  views::Label* CreateLabel(int message_id);

  
  
  views::Textfield* CreateTextfield(const std::wstring& text, bool lowercase);

  
  void UpdateImageViews();

  
  
  
  void UpdateImageView(views::ImageView* image_view,
                       bool is_valid,
                       int invalid_message_id);

  
  HWND parent_;

  
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
