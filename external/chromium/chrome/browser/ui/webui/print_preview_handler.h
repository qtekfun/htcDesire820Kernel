// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_PRINT_PREVIEW_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_PRINT_PREVIEW_HANDLER_H_
#pragma once

#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/ui/shell_dialogs.h"
#include "content/browser/webui/web_ui.h"

class FilePath;
class PrintSystemTaskProxy;
class FundamentalValue;

namespace printing {
class PrintBackend;
}

class PrintPreviewHandler : public WebUIMessageHandler,
                            public base::SupportsWeakPtr<PrintPreviewHandler>,
                            public SelectFileDialog::Listener {
 public:
  PrintPreviewHandler();
  virtual ~PrintPreviewHandler();

  
  virtual void RegisterMessages();

  
  virtual void FileSelected(const FilePath& path, int index, void* params);

  
  void SelectFile(const FilePath& default_path);

 private:
  friend class PrintSystemTaskProxy;

  
  void HandleGetPrinters(const ListValue* args);

  
  
  void HandleGetPreview(const ListValue* args);

  
  
  void HandlePrint(const ListValue* args);

  
  
  void HandleGetPrinterCapabilities(const ListValue* args);

  
  
  void SendPrinterCapabilities(const DictionaryValue& settings_info);

  
  void SendPrinterList(const ListValue& printers,
                       const FundamentalValue& default_printer_index);

  
  void ProcessColorSetting(const DictionaryValue& settings);

  
  void ProcessLandscapeSetting(const DictionaryValue& settings);

  
  scoped_refptr<printing::PrintBackend> print_backend_;

  
  scoped_refptr<SelectFileDialog> select_file_dialog_;

  static FilePath* last_saved_path_;

  DISALLOW_COPY_AND_ASSIGN(PrintPreviewHandler);
};

#endif  
