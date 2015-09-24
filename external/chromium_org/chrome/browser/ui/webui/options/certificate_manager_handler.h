// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_CERTIFICATE_MANAGER_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_CERTIFICATE_MANAGER_HANDLER_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/certificate_manager_model.h"
#include "chrome/browser/ui/webui/options/options_ui.h"
#include "chrome/common/cancelable_task_tracker.h"
#include "net/cert/nss_cert_database.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/shell_dialogs/select_file_dialog.h"

#if defined(OS_CHROMEOS)
#include "chromeos/dbus/cryptohome_client.h"
#endif

namespace options {

class CertIdMap;
class FileAccessProvider;

class CertificateManagerHandler
    : public OptionsPageUIHandler,
      public CertificateManagerModel::Observer,
      public ui::SelectFileDialog::Listener {
 public:
  CertificateManagerHandler();
  virtual ~CertificateManagerHandler();

  
  virtual void GetLocalizedValues(
      base::DictionaryValue* localized_strings) OVERRIDE;
  virtual void RegisterMessages() OVERRIDE;

  
  virtual void CertificatesRefreshed() OVERRIDE;

  
  virtual void FileSelected(const base::FilePath& path,
                            int index,
                            void* params) OVERRIDE;
  virtual void FileSelectionCanceled(void* params) OVERRIDE;

 private:
  
  void View(const base::ListValue* args);

  
  void EditServer(const base::ListValue* args);

  
  
  
  
  void GetCATrust(const base::ListValue* args);
  void EditCATrust(const base::ListValue* args);

  
  void CancelImportExportProcess(const base::ListValue* args);
  void ImportExportCleanup();

  
  
  
  
  
  
  
  
  
  
  //  5. write finishes (or fails) -> ExportPersonalFileWritten
  void ExportPersonal(const base::ListValue* args);
  void ExportAllPersonal(const base::ListValue* args);
  void ExportPersonalFileSelected(const base::FilePath& path);
  void ExportPersonalPasswordSelected(const base::ListValue* args);
  void ExportPersonalSlotsUnlocked();
  void ExportPersonalFileWritten(const int* write_errno,
                                 const int* bytes_written);

  
  
  
  
  
  
  
  
  
  
  
  
  
  void StartImportPersonal(const base::ListValue* args);
  void ImportPersonalFileSelected(const base::FilePath& path);
  void ImportPersonalPasswordSelected(const base::ListValue* args);
  void ImportPersonalFileRead(const int* read_errno, const std::string* data);
  void ImportPersonalSlotUnlocked();

  
  
  
  
  
  
  void ImportServer(const base::ListValue* args);
  void ImportServerFileSelected(const base::FilePath& path);
  void ImportServerFileRead(const int* read_errno, const std::string* data);

  
  
  
  
  
  
  
  
  void ImportCA(const base::ListValue* args);
  void ImportCAFileSelected(const base::FilePath& path);
  void ImportCAFileRead(const int* read_errno, const std::string* data);
  void ImportCATrustSelected(const base::ListValue* args);

  
  void Export(const base::ListValue* args);

  
  void Delete(const base::ListValue* args);

  
  void Populate(const base::ListValue* args);

  
  void PopulateTree(const std::string& tab_name,
                    net::CertType type,
                    const net::CertificateList& web_trust_certs);

  
  
  void OnPolicyWebTrustCertsRetrieved(
      const net::CertificateList& web_trust_certs);

  
  void ShowError(const std::string& title, const std::string& error) const;

  
  
  
  void ShowImportErrors(
      const std::string& title,
      const net::NSSCertDatabase::ImportCertFailureList& not_imported) const;

#if defined(OS_CHROMEOS)
  
  void CheckTpmTokenReady(const base::ListValue* args);
  void CheckTpmTokenReadyInternal(
      chromeos::DBusMethodCallStatus call_status,
      bool is_tpm_token_ready);
#endif

  gfx::NativeWindow GetParentWindow() const;

  
  scoped_ptr<CertificateManagerModel> certificate_manager_model_;

  
  
  
  base::FilePath file_path_;
  base::string16 password_;
  bool use_hardware_backed_;
  std::string file_data_;
  net::CertificateList selected_cert_list_;
  scoped_refptr<ui::SelectFileDialog> select_file_dialog_;
  scoped_refptr<net::CryptoModule> module_;

  
  CancelableTaskTracker tracker_;
  scoped_refptr<FileAccessProvider> file_access_provider_;

  scoped_ptr<CertIdMap> cert_id_map_;

  base::WeakPtrFactory<CertificateManagerHandler> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(CertificateManagerHandler);
};

}  

#endif  
