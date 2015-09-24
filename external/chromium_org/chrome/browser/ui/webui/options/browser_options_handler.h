// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_BROWSER_OPTIONS_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_BROWSER_OPTIONS_HANDLER_H_

#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/prefs/pref_change_registrar.h"
#include "base/prefs/pref_member.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/search_engines/template_url_service_observer.h"
#include "chrome/browser/shell_integration.h"
#include "chrome/browser/sync/profile_sync_service_observer.h"
#include "chrome/browser/ui/host_desktop.h"
#include "chrome/browser/ui/webui/options/options_ui.h"
#include "google_apis/gaia/google_service_auth_error.h"
#include "ui/base/models/table_model_observer.h"
#include "ui/shell_dialogs/select_file_dialog.h"

#if defined(OS_CHROMEOS)
#include "chrome/browser/chromeos/system/pointer_device_observer.h"
#endif  

class AutocompleteController;
class CloudPrintSetupHandler;
class CustomHomePagesTableModel;
class TemplateURLService;

namespace base {
class Value;
}

namespace policy {
class PolicyChangeRegistrar;
}

namespace options {

class BrowserOptionsHandler
    : public OptionsPageUIHandler,
      public ProfileSyncServiceObserver,
      public ui::SelectFileDialog::Listener,
      public ShellIntegration::DefaultWebClientObserver,
#if defined(OS_CHROMEOS)
      public chromeos::system::PointerDeviceObserver::Observer,
#endif
      public TemplateURLServiceObserver {
 public:
  BrowserOptionsHandler();
  virtual ~BrowserOptionsHandler();

  
  virtual void GetLocalizedValues(DictionaryValue* values) OVERRIDE;
  virtual void PageLoadStarted() OVERRIDE;
  virtual void InitializeHandler() OVERRIDE;
  virtual void InitializePage() OVERRIDE;
  virtual void RegisterMessages() OVERRIDE;
  virtual void Uninitialize() OVERRIDE;

  
  virtual void OnStateChanged() OVERRIDE;

  
  virtual void SetDefaultWebClientUIState(
      ShellIntegration::DefaultWebClientUIState state) OVERRIDE;
  virtual bool IsInteractiveSetDefaultPermitted() OVERRIDE;

  
  virtual void OnTemplateURLServiceChanged() OVERRIDE;

 private:
  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

#if defined(ENABLE_FULL_PRINTING) && !defined(OS_CHROMEOS)
  void OnCloudPrintPrefsChanged();
#endif

  
  virtual void FileSelected(const base::FilePath& path,
                            int index,
                            void* params) OVERRIDE;

#if defined(OS_CHROMEOS)
  
  virtual void TouchpadExists(bool exists) OVERRIDE;
  virtual void MouseExists(bool exists) OVERRIDE;

  
  void OnUserImagePolicyChanged(const base::Value* previous_policy,
                                const base::Value* current_policy);
#endif

  void UpdateSyncState();

  
  void OnSigninAllowedPrefChange();

  
  void BecomeDefaultBrowser(const base::ListValue* args);

  
  void SetDefaultSearchEngine(const base::ListValue* args);

  
  void ToggleAutoLaunch(const base::ListValue* args);

  
  
  
  
  
  static void CheckAutoLaunch(base::WeakPtr<BrowserOptionsHandler> weak_this,
                              const base::FilePath& profile_path);

  
  
  
  void CheckAutoLaunchCallback(bool is_in_auto_launch_group,
                               bool will_launch_at_login);

  
  int StatusStringIdForState(ShellIntegration::DefaultWebClientState state);

  
  
  void UpdateDefaultBrowserState();

  
  void SetDefaultBrowserUIString(int status_string_id);

  
  void AddTemplateUrlServiceObserver();

  
  
  
  
  
  
  
  scoped_ptr<ListValue> GetProfilesInfoList();

  
  void SendProfilesInfo();

  
  
  void DeleteProfile(const base::ListValue* args);

  void ObserveThemeChanged();
  void ThemesReset(const base::ListValue* args);
#if defined(OS_LINUX) && !defined(OS_CHROMEOS)
  void ThemesSetNative(const base::ListValue* args);
#endif

#if defined(OS_CHROMEOS)
  void UpdateAccountPicture();

  
  
  
  void OnAccountPictureManagedChanged(bool managed);
#endif

  
  
  void HandleSelectDownloadLocation(const ListValue* args);

  
  
  void HandleAutoOpenButton(const ListValue* args);

  
  
  
  void HandleDefaultFontSize(const ListValue* args);

  
  
  
  void HandleDefaultZoomFactor(const ListValue* args);

  
  
  void HandleUseSSL3Checkbox(const ListValue* args);

  
  
  void HandleUseTLS1Checkbox(const ListValue* args);

  
  void HandleRestartBrowser(const ListValue* args);

  
  void HandleRequestProfilesInfo(const ListValue* args);

#if !defined(OS_CHROMEOS)
  
  
  void ShowNetworkProxySettings(const ListValue* args);
#endif

#if !defined(USE_NSS)
  
  
  void ShowManageSSLCertificates(const ListValue* args);
#endif

#if defined(ENABLE_MDNS)
  void ShowCloudPrintDevicesPage(const ListValue* args);
#endif

#if defined(ENABLE_FULL_PRINTING)
  
  
  void ShowCloudPrintManagePage(const ListValue* args);

  
  void RegisterCloudPrintValues(DictionaryValue* values);

#if !defined(OS_CHROMEOS)
  
  
  void ShowCloudPrintSetupDialog(const ListValue* args);

  
  
  void HandleDisableCloudPrintConnector(const ListValue* args);

  
  void RefreshCloudPrintStatusFromService();

  
  
  void SetupCloudPrintConnectorSection();

  
  
  void RemoveCloudPrintConnectorSection();
#endif  
#endif  

#if defined(OS_CHROMEOS)
  
  void HandleOpenWallpaperManager(const base::ListValue* args);

  
  
  
  void VirtualKeyboardChangeCallback(const base::ListValue* args);

  
  
  void PerformFactoryResetRestart(const base::ListValue* args);
#endif

  
  void SetupMetricsReportingSettingVisibility();

  
  void SetupPasswordGenerationSettingVisibility();

  
  void SetupFontSizeSelector();

  
  void SetupPageZoomSelector();

  
  void SetupAutoOpenFileTypes();

  
  void SetupProxySettingsSection();

  
  void SetupManageCertificatesSection();

  
  void SetupManagingSupervisedUsers();

#if defined(OS_CHROMEOS)
  
  void SetupAccessibilityFeatures();
#endif

  
  
  scoped_ptr<DictionaryValue> GetSyncStateDictionary();

  scoped_refptr<ShellIntegration::DefaultBrowserWorker> default_browser_worker_;

  bool page_initialized_;

  StringPrefMember homepage_;
  BooleanPrefMember default_browser_policy_;

  TemplateURLService* template_url_service_;  

  scoped_refptr<ui::SelectFileDialog> select_folder_dialog_;

#if defined(ENABLE_FULL_PRINTING) && !defined(OS_CHROMEOS)
  StringPrefMember cloud_print_connector_email_;
  BooleanPrefMember cloud_print_connector_enabled_;
  bool cloud_print_connector_ui_enabled_;
#endif

  bool cloud_print_mdns_ui_enabled_;

  StringPrefMember auto_open_files_;
  DoublePrefMember default_zoom_level_;

  PrefChangeRegistrar profile_pref_registrar_;
#if defined(OS_CHROMEOS)
  scoped_ptr<policy::PolicyChangeRegistrar> policy_registrar_;
#endif

  
  base::WeakPtrFactory<BrowserOptionsHandler> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(BrowserOptionsHandler);
};

}  

#endif  
