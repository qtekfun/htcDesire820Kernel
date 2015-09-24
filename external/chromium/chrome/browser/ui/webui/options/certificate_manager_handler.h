// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_CERTIFICATE_MANAGER_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_CERTIFICATE_MANAGER_HANDLER_H_
#pragma once

#include <string>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/shell_dialogs.h"
#include "chrome/browser/ui/webui/options/options_ui.h"
#include "content/browser/cancelable_request.h"
#include "content/browser/certificate_manager_model.h"
#include "net/base/cert_database.h"
#include "ui/gfx/native_widget_types.h"

class FileAccessProvider;

class CertificateManagerHandler : public OptionsPageUIHandler,
    public CertificateManagerModel::Observer,
    public SelectFileDialog::Listener {
 public:
  CertificateManagerHandler();
  virtual ~CertificateManagerHandler();

  
  virtual void GetLocalizedValues(DictionaryValue* localized_strings);
  virtual void RegisterMessages();

  
  virtual void CertificatesRefreshed();

  
  virtual void FileSelected(const FilePath& path, int index, void* params);
  virtual void FileSelectionCanceled(void* params);

 private:
  
  void View(const ListValue* args);

  
  void EditServer(const ListValue* args);

  
  
  
  
  void GetCATrust(const ListValue* args);
  void EditCATrust(const ListValue* args);

  
  void CancelImportExportProcess(const ListValue* args);
  void ImportExportCleanup();

  
  
  
  
  
  
  
  
  
  
  //  5. write finishes (or fails) -> ExportPersonalFileWritten
  void ExportPersonal(const ListValue* args);
  void ExportAllPersonal(const ListValue* args);
  void ExportPersonalFileSelected(const FilePath& path);
  void ExportPersonalPasswordSelected(const ListValue* args);
  void ExportPersonalSlotsUnlocked();
  void ExportPersonalFileWritten(int write_errno, int bytes_written);

  
  
  
  
  
  
  
  
  
  
  
  
  
  void StartImportPersonal(const ListValue* args);
  void ImportPersonalFileSelected(const FilePath& path);
  void ImportPersonalPasswordSelected(const ListValue* args);
  void ImportPersonalFileRead(int read_errno, std::string data);
  void ImportPersonalSlotUnlocked();

  
  
  
  
  
  
  void ImportServer(const ListValue* args);
  void ImportServerFileSelected(const FilePath& path);
  void ImportServerFileRead(int read_errno, std::string data);

  
  
  
  
  
  
  
  
  void ImportCA(const ListValue* args);
  void ImportCAFileSelected(const FilePath& path);
  void ImportCAFileRead(int read_errno, std::string data);
  void ImportCATrustSelected(const ListValue* args);

  
  void Export(const ListValue* args);

  
  void Delete(const ListValue* args);

  
  void Populate(const ListValue* args);

  
  void PopulateTree(const std::string& tab_name, net::CertType type);

  
  void ShowError(const std::string& title, const std::string& error) const;

  
  
  
  void ShowImportErrors(
      const std::string& title,
      const net::CertDatabase::ImportCertFailureList& not_imported) const;

#if defined(OS_CHROMEOS)
  
  void CheckTpmTokenReady(const ListValue* args);
#endif

  gfx::NativeWindow GetParentWindow() const;

  
  scoped_ptr<CertificateManagerModel> certificate_manager_model_;

  
  
  
  FilePath file_path_;
  string16 password_;
  bool use_hardware_backed_;
  std::string file_data_;
  net::CertificateList selected_cert_list_;
  scoped_refptr<SelectFileDialog> select_file_dialog_;
  scoped_refptr<net::CryptoModule> module_;

  
  CancelableRequestConsumer consumer_;
  scoped_refptr<FileAccessProvider> file_access_provider_;

  DISALLOW_COPY_AND_ASSIGN(CertificateManagerHandler);
};

#endif  
