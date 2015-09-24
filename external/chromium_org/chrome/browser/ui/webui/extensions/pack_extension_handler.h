// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_EXTENSIONS_PACK_EXTENSION_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_EXTENSIONS_PACK_EXTENSION_HANDLER_H_

#include <string>

#include "base/files/file_path.h"
#include "chrome/browser/browsing_data/browsing_data_remover.h"
#include "chrome/browser/extensions/pack_extension_job.h"
#include "chrome/browser/plugins/plugin_data_remover_helper.h"
#include "content/public/browser/web_ui_message_handler.h"
#include "ui/shell_dialogs/select_file_dialog.h"

namespace content {
class WebUIDataSource;
}

namespace extensions {

class PackExtensionHandler : public content::WebUIMessageHandler,
                             public ui::SelectFileDialog::Listener,
                             public PackExtensionJob::Client {
 public:
  PackExtensionHandler();
  virtual ~PackExtensionHandler();

  void GetLocalizedValues(content::WebUIDataSource* source);

  
  virtual void RegisterMessages() OVERRIDE;

  
  virtual void OnPackSuccess(const base::FilePath& crx_file,
                             const base::FilePath& key_file) OVERRIDE;

  virtual void OnPackFailure(const std::string& error,
                             ExtensionCreator::ErrorType) OVERRIDE;

 private:
  
  virtual void FileSelected(const base::FilePath& path,
                            int index, void* params) OVERRIDE;
  virtual void MultiFilesSelected(
      const std::vector<base::FilePath>& files, void* params) OVERRIDE;
  virtual void FileSelectionCanceled(void* params) OVERRIDE {}

  
  void HandlePackMessage(const ListValue* args);

  
  
  
  
  
  void HandleSelectFilePathMessage(const ListValue* args);

  
  void ShowAlert(const std::string& message);

  
  scoped_refptr<PackExtensionJob> pack_job_;

  
  
  scoped_refptr<ui::SelectFileDialog> load_extension_dialog_;

  
  base::FilePath extension_path_;

  
  base::FilePath private_key_path_;

  
  base::FilePath last_used_path_;

  DISALLOW_COPY_AND_ASSIGN(PackExtensionHandler);
};

}  

#endif  
