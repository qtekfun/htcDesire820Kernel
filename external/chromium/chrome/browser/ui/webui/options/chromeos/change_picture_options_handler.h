// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_CHROMEOS_CHANGE_PICTURE_OPTIONS_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_CHROMEOS_CHANGE_PICTURE_OPTIONS_HANDLER_H_

#include "chrome/browser/ui/shell_dialogs.h"
#include "chrome/browser/ui/webui/options/options_ui.h"
#include "ui/gfx/native_widget_types.h"

class DictionaryValue;
class ListValue;

namespace chromeos {

class ChangePictureOptionsHandler : public OptionsPageUIHandler,
                                    public SelectFileDialog::Listener {
 public:
  ChangePictureOptionsHandler();
  virtual ~ChangePictureOptionsHandler();

  
  virtual void GetLocalizedValues(DictionaryValue* localized_strings);

  
  virtual void RegisterMessages();

 private:
  
  void ChooseFile(const ListValue* args);

  
  void TakePhoto(const ListValue* args);

  
  void GetAvailableImages(const ListValue* args);

  
  void SelectImage(const ListValue* args);

  
  virtual void FileSelected(const FilePath& path, int index, void* params);

  
  gfx::NativeWindow GetBrowserWindow() const;

  scoped_refptr<SelectFileDialog> select_file_dialog_;

  DISALLOW_COPY_AND_ASSIGN(ChangePictureOptionsHandler);
};

} 

#endif  
