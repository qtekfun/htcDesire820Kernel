// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_CRX_INSTALLER_H_
#define CHROME_BROWSER_EXTENSIONS_CRX_INSTALLER_H_
#pragma once

#include <string>

#include "base/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/version.h"
#include "chrome/browser/extensions/extension_install_ui.h"
#include "chrome/browser/extensions/sandboxed_extension_unpacker.h"
#include "chrome/common/extensions/extension.h"
#include "chrome/common/web_apps.h"

class ExtensionService;
class SkBitmap;

class CrxInstaller
    : public SandboxedExtensionUnpackerClient,
      public ExtensionInstallUI::Delegate {
 public:

  
  
  
  
  
  
  static void SetWhitelistedInstallId(const std::string& id);

  
  
  
  
  
  static void SetWhitelistedManifest(const std::string& id,
                                     DictionaryValue* parsed_manifest);

  
  
  static const DictionaryValue* GetWhitelistedManifest(const std::string& id);

  
  
  static DictionaryValue* RemoveWhitelistedManifest(const std::string& id);

  
  static bool IsIdWhitelisted(const std::string& id);

  
  
  static bool ClearWhitelistedInstallId(const std::string& id);

  
  
  
  
  CrxInstaller(ExtensionService* frontend,
               ExtensionInstallUI* client);

  
  void InstallCrx(const FilePath& source_file);

  
  void InstallUserScript(const FilePath& source_file,
                         const GURL& original_url);

  
  void InstallWebApp(const WebApplicationInfo& web_app);

  
  virtual void InstallUIProceed();
  virtual void InstallUIAbort();

  const GURL& original_url() const { return original_url_; }
  void set_original_url(const GURL& val) { original_url_ = val; }

  Extension::Location install_source() const { return install_source_; }
  void set_install_source(Extension::Location source) {
    install_source_ = source;
  }

  const std::string& expected_id() const { return expected_id_; }
  void set_expected_id(const std::string& val) { expected_id_ = val; }

  void set_expected_version(const Version& val) {
    expected_version_.reset(val.Clone());
  }

  bool delete_source() const { return delete_source_; }
  void set_delete_source(bool val) { delete_source_ = val; }

  bool allow_silent_install() const { return allow_silent_install_; }
  void set_allow_silent_install(bool val) { allow_silent_install_ = val; }

  bool is_gallery_install() const { return is_gallery_install_; }
  void set_is_gallery_install(bool val) { is_gallery_install_ = val; }

  
  
  void set_apps_require_extension_mime_type(
      bool apps_require_extension_mime_type) {
    apps_require_extension_mime_type_ = apps_require_extension_mime_type;
  }

  void set_original_mime_type(const std::string& original_mime_type) {
    original_mime_type_ = original_mime_type;
  }

 private:
  ~CrxInstaller();

  
  void ConvertUserScriptOnFileThread();

  
  void ConvertWebAppOnFileThread(const WebApplicationInfo& web_app);

  
  
  bool AllowInstall(const Extension* extension, std::string* error);

  
  virtual void OnUnpackFailure(const std::string& error_message);
  virtual void OnUnpackSuccess(const FilePath& temp_dir,
                               const FilePath& extension_dir,
                               const Extension* extension);

  
  
  bool CanSkipConfirmation();

  
  
  void ConfirmInstall();

  
  
  void CompleteInstall();

  
  void ReportFailureFromFileThread(const std::string& error);
  void ReportFailureFromUIThread(const std::string& error);
  void ReportSuccessFromFileThread();
  void ReportSuccessFromUIThread();

  
  FilePath source_file_;

  
  GURL original_url_;

  
  FilePath install_directory_;

  
  
  
  Extension::Location install_source_;

  
  
  std::string expected_id_;

  
  
  
  
  scoped_ptr<Version> expected_version_;

  
  
  
  bool extensions_enabled_;

  
  
  bool delete_source_;

  
  bool is_gallery_install_;

  
  
  
  bool create_app_shortcut_;

  
  
  scoped_refptr<const Extension> extension_;

  
  
  std::string current_version_;

  
  scoped_ptr<SkBitmap> install_icon_;

  
  
  FilePath temp_dir_;

  
  scoped_refptr<ExtensionService> frontend_;

  
  
  
  
  ExtensionInstallUI* client_;

  
  
  FilePath unpacked_extension_root_;

  
  
  bool apps_require_extension_mime_type_;

  
  
  
  
  
  bool allow_silent_install_;

  
  
  std::string original_mime_type_;

  DISALLOW_COPY_AND_ASSIGN(CrxInstaller);
};

#endif  
