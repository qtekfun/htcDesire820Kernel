// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_ADVANCED_OPTIONS_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_ADVANCED_OPTIONS_HANDLER_H_
#pragma once

#include "chrome/browser/prefs/pref_member.h"
#include "chrome/browser/prefs/pref_set_observer.h"
#include "chrome/browser/printing/cloud_print/cloud_print_setup_handler.h"
#include "chrome/browser/remoting/remoting_options_handler.h"
#include "chrome/browser/ui/shell_dialogs.h"
#include "chrome/browser/ui/webui/options/options_ui.h"

class OptionsManagedBannerHandler;
class CloudPrintSetupHandler;

class AdvancedOptionsHandler
    : public OptionsPageUIHandler,
      public SelectFileDialog::Listener,
      public CloudPrintSetupHandlerDelegate {
 public:
  AdvancedOptionsHandler();
  virtual ~AdvancedOptionsHandler();

  
  virtual void GetLocalizedValues(DictionaryValue* localized_strings);
  virtual void Initialize();

  
  virtual WebUIMessageHandler* Attach(WebUI* web_ui);
  virtual void RegisterMessages();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  virtual void FileSelected(const FilePath& path, int index, void* params);

  
  virtual void OnCloudPrintSetupClosed();

 private:
  
  
  void HandleSelectDownloadLocation(const ListValue* args);

  
  
  void HandlePromptForDownload(const ListValue* args);

  
  
  void HandleAutoOpenButton(const ListValue* args);

  
  
  void HandleMetricsReportingCheckbox(const ListValue* args);

  
  
  
  void HandleDefaultFontSize(const ListValue* args);

  
  
  
  void HandleCheckRevocationCheckbox(const ListValue* args);

  
  
  void HandleUseSSL3Checkbox(const ListValue* args);

  
  
  void HandleUseTLS1Checkbox(const ListValue* args);

#if !defined(OS_CHROMEOS)
  
  
  void ShowNetworkProxySettings(const ListValue* args);
#endif

#if !defined(USE_NSS)
  
  
  void ShowManageSSLCertificates(const ListValue* args);
#endif

#if !defined(OS_CHROMEOS)
  
  
  void ShowCloudPrintSetupDialog(const ListValue* args);

  
  
  void HandleDisableCloudPrintProxy(const ListValue* args);

  
  
  void ShowCloudPrintManagePage(const ListValue* args);

  
  void RefreshCloudPrintStatusFromService();

  
  
  void SetupCloudPrintProxySection();

  
  
  void RemoveCloudPrintProxySection();

#endif

#if defined(ENABLE_REMOTING) && !defined(OS_CHROMEOS)
  
  void RemoveRemotingSection();

  
  void ShowRemotingSetupDialog(const ListValue* args);

  
  void DisableRemoting(const ListValue* args);
#endif

  
  void SetupMetricsReportingCheckbox();

  
  void SetupMetricsReportingSettingVisibility();

  void SetupFontSizeLabel();

  
  void SetupDownloadLocationPath();

  
  void SetupPromptForDownload();

  
  void SetupAutoOpenFileTypesDisabledAttribute();

  
  void SetupProxySettingsSection();

  
  void SetupSSLConfigSettings();

  scoped_refptr<SelectFileDialog> select_folder_dialog_;

#if !defined(OS_CHROMEOS)
  BooleanPrefMember enable_metrics_recording_;
  StringPrefMember cloud_print_proxy_email_;
  BooleanPrefMember cloud_print_proxy_enabled_;
  bool cloud_print_proxy_ui_enabled_;
  scoped_ptr<CloudPrintSetupHandler> cloud_print_setup_handler_;
#endif

  
  BooleanPrefMember rev_checking_enabled_;
  BooleanPrefMember ssl3_enabled_;
  BooleanPrefMember tls1_enabled_;

#if defined(ENABLE_REMOTING) && !defined(OS_CHROMEOS)
  remoting::RemotingOptionsHandler remoting_options_handler_;
#endif

  FilePathPrefMember default_download_location_;
  BooleanPrefMember ask_for_save_location_;
  StringPrefMember auto_open_files_;
  IntegerPrefMember default_font_size_;
  scoped_ptr<PrefSetObserver> proxy_prefs_;
  scoped_ptr<OptionsManagedBannerHandler> banner_handler_;

  DISALLOW_COPY_AND_ASSIGN(AdvancedOptionsHandler);
};

#endif  
