// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_EDIT_SEARCH_ENGINE_DIALOG_H_
#define CHROME_BROWSER_UI_GTK_EDIT_SEARCH_ENGINE_DIALOG_H_

#include <gtk/gtk.h>
#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "ui/base/gtk/gtk_signal.h"

class EditSearchEngineController;
class EditSearchEngineControllerDelegate;
class Profile;
class TemplateURL;

class EditSearchEngineDialog {
 public:
  EditSearchEngineDialog(GtkWindow* parent_window,
                         TemplateURL* template_url,
                         EditSearchEngineControllerDelegate* delegate,
                         Profile* profile);
  ~EditSearchEngineDialog();

 private:
  
  void Init(GtkWindow* parent_window, Profile* profile);

  
  base::string16 GetTitleInput() const;
  base::string16 GetKeywordInput() const;
  std::string GetURLInput() const;

  
  void EnableControls();

  
  
  
  void UpdateImage(GtkWidget* image, bool is_valid, int invalid_message_id);

  
  CHROMEG_CALLBACK_0(EditSearchEngineDialog, void, OnEntryChanged,
                     GtkEditable*);

  
  CHROMEGTK_CALLBACK_1(EditSearchEngineDialog, void, OnResponse, int);

  
  CHROMEGTK_CALLBACK_0(EditSearchEngineDialog, void, OnWindowDestroy);

  
  GtkWidget* dialog_;

  
  GtkWidget* title_entry_;
  GtkWidget* keyword_entry_;
  GtkWidget* url_entry_;

  
  GtkWidget* title_image_;
  GtkWidget* keyword_image_;
  GtkWidget* url_image_;

  
  
  GtkWidget* ok_button_;

  scoped_ptr<EditSearchEngineController> controller_;

  DISALLOW_COPY_AND_ASSIGN(EditSearchEngineDialog);
};

#endif  
