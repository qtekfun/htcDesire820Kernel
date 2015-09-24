// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_SERVICE_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_SERVICE_H_
#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/command_line.h"
#include "base/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/ref_counted.h"
#include "base/task.h"
#include "base/time.h"
#include "base/tuple.h"
#include "base/version.h"
#include "chrome/browser/extensions/apps_promo.h"
#include "chrome/browser/extensions/extension_icon_manager.h"
#include "chrome/browser/extensions/extension_menu_manager.h"
#include "chrome/browser/extensions/extension_prefs.h"
#include "chrome/browser/extensions/extension_process_manager.h"
#include "chrome/browser/extensions/extension_toolbar_model.h"
#include "chrome/browser/extensions/extensions_quota_service.h"
#include "chrome/browser/extensions/external_extension_provider_interface.h"
#include "chrome/browser/extensions/pending_extension_manager.h"
#include "chrome/browser/extensions/sandboxed_extension_unpacker.h"
#include "chrome/browser/prefs/pref_change_registrar.h"
#include "chrome/common/extensions/extension.h"
#include "content/browser/browser_thread.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "content/common/property_bag.h"

class ExtensionBrowserEventRouter;
class ExtensionPreferenceEventRouter;
class ExtensionServiceBackend;
struct ExtensionSyncData;
class ExtensionToolbarModel;
class ExtensionUpdater;
class GURL;
class PendingExtensionManager;
class Profile;
class Version;

class ExtensionServiceInterface {
 public:
  virtual ~ExtensionServiceInterface() {}
  virtual const ExtensionList* extensions() const = 0;
  virtual const ExtensionList* disabled_extensions() const = 0;
  virtual PendingExtensionManager* pending_extension_manager() = 0;
  virtual void UpdateExtension(const std::string& id,
                               const FilePath& path,
                               const GURL& download_url) = 0;
  virtual const Extension* GetExtensionById(const std::string& id,
                                            bool include_disabled) const = 0;

  virtual bool UninstallExtension(const std::string& extension_id,
                                  bool external_uninstall,
                                  std::string* error) = 0;

  virtual bool IsExtensionEnabled(const std::string& extension_id) const = 0;
  virtual bool IsExternalExtensionUninstalled(
      const std::string& extension_id) const = 0;
  virtual void EnableExtension(const std::string& extension_id) = 0;
  virtual void DisableExtension(const std::string& extension_id) = 0;

  virtual void UpdateExtensionBlacklist(
    const std::vector<std::string>& blacklist) = 0;
  virtual void CheckAdminBlacklist() = 0;

  virtual bool IsIncognitoEnabled(const std::string& extension_id) const = 0;
  virtual void SetIsIncognitoEnabled(const std::string& extension_id,
                                     bool enabled) = 0;

  
  
  
  
  virtual void CheckForUpdatesSoon() = 0;

  
  
  
  
  
  
  virtual void ProcessSyncData(
      const ExtensionSyncData& extension_sync_data,
      PendingExtensionInfo::ShouldAllowInstallPredicate should_allow) = 0;

  
  
  
  
  
  
  
  
  
};

class ExtensionService
    : public base::RefCountedThreadSafe<ExtensionService,
                                        BrowserThread::DeleteOnUIThread>,
      public ExtensionServiceInterface,
      public ExternalExtensionProviderInterface::VisitorInterface,
      public NotificationObserver {
 public:
  
  struct ComponentExtensionInfo {
    ComponentExtensionInfo(const std::string& manifest,
                           const FilePath& root_directory)
        : manifest(manifest),
          root_directory(root_directory) {
    }

    
    
    std::string manifest;

    
    FilePath root_directory;
  };

  
  
  static const char* kInstallDirectoryName;

  
  static const int kDefaultUpdateFrequencySeconds = 60 * 60 * 5;

  
  static const char* kCurrentVersionFileName;

  
  
  
  bool IsDownloadFromGallery(const GURL& download_url,
                             const GURL& referrer_url);

  
  
  static bool IsDownloadFromMiniGallery(const GURL& download_url);

  
  const Extension* GetInstalledApp(const GURL& url);

  
  bool IsInstalledApp(const GURL& url);

  
  
  static bool UninstallExtensionHelper(ExtensionService* extensions_service,
                                       const std::string& extension_id);

  
  
  ExtensionService(Profile* profile,
                   const CommandLine* command_line,
                   const FilePath& install_directory,
                   ExtensionPrefs* extension_prefs,
                   bool autoupdate_enabled,
                   bool extensions_enabled);

  
  virtual const ExtensionList* extensions() const;
  virtual const ExtensionList* disabled_extensions() const;
  virtual const ExtensionList* terminated_extensions() const;

  
  virtual PendingExtensionManager* pending_extension_manager();

  
  void register_component_extension(const ComponentExtensionInfo& info) {
    component_extension_manifests_.push_back(info);
  }

  const FilePath& install_directory() const { return install_directory_; }

  AppsPromo* apps_promo() { return &apps_promo_; }

  
  virtual bool IsIncognitoEnabled(const std::string& extension_id) const;
  virtual void SetIsIncognitoEnabled(const std::string& extension_id,
                                     bool enabled);

  
  
  bool CanCrossIncognito(const Extension* extension);

  
  bool AllowFileAccess(const Extension* extension);
  
  
  void SetAllowFileAccess(const Extension* extension, bool allow);

  
  bool GetBrowserActionVisibility(const Extension* extension);
  void SetBrowserActionVisibility(const Extension* extension, bool visible);

  
  
  
  bool IsBackgroundPageReady(const Extension* extension);
  void SetBackgroundPageReady(const Extension* extension);

  
  
  bool IsBeingUpgraded(const Extension* extension);
  void SetBeingUpgraded(const Extension* extension, bool value);

  
  PropertyBag* GetPropertyBag(const Extension* extension);

  
  void Init();

  
  void InitEventRouters();

  
  virtual const Extension* GetExtensionById(const std::string& id,
                                            bool include_disabled) const;

  
  
  virtual const Extension* GetTerminatedExtension(const std::string& id);

  
  
  
  
  virtual void UpdateExtension(const std::string& id,
                               const FilePath& extension_path,
                               const GURL& download_url);

  
  void ReloadExtension(const std::string& extension_id);

  
  
  
  
  
  
  virtual bool UninstallExtension(const std::string& extension_id,
                                  bool external_uninstall,
                                  std::string* error);

  virtual bool IsExtensionEnabled(const std::string& extension_id) const;
  virtual bool IsExternalExtensionUninstalled(
      const std::string& extension_id) const;

  
  
  virtual void EnableExtension(const std::string& extension_id);
  virtual void DisableExtension(const std::string& extension_id);

  
  
  void GrantPermissions(const Extension* extension);

  
  
  
  void GrantPermissionsAndEnableExtension(const Extension* extension);

  
  void LoadExtension(const FilePath& extension_path);

  
  void LoadComponentExtensions();

  
  const Extension* LoadComponentExtension(const ComponentExtensionInfo& info);

  
  void LoadAllExtensions();

  
  
  
  void ContinueLoadAllExtensions(ExtensionPrefs::ExtensionsInfo* info,
                                 base::TimeTicks start_time,
                                 bool write_to_prefs);

  
  void CheckForExternalUpdates();

  
  void UnloadExtension(const std::string& extension_id,
                       UnloadedExtensionInfo::Reason reason);

  
  
  void UnloadAllExtensions();

  
  void ReloadExtensions();

  
  void GarbageCollectExtensions();

  
  const Extension* GetWebStoreApp();

  
  const Extension* GetExtensionByURL(const GURL& url);

  
  
  const Extension* GetExtensionByWebExtent(const GURL& url);

  
  
  const Extension* GetExtensionByOverlappingWebExtent(
      const ExtensionExtent& extent);

  
  
  
  bool ExtensionBindingsAllowed(const GURL& url);

  
  const SkBitmap& GetOmniboxIcon(const std::string& extension_id);

  
  
  const SkBitmap& GetOmniboxPopupIcon(const std::string& extension_id);

  
  virtual void OnLoadedInstalledExtensions();

  
  
  
  void AddExtension(const Extension* extension);

  
  void OnExtensionInstalled(const Extension* extension);

  
  
  void DisableIfPrivilegeIncrease(const Extension* extension);

  
  
  virtual void UpdateExtensionBlacklist(
    const std::vector<std::string>& blacklist);

  
  
  
  virtual void CheckAdminBlacklist();

  virtual void CheckForUpdatesSoon();

  virtual void ProcessSyncData(
      const ExtensionSyncData& extension_sync_data,
      PendingExtensionInfo::ShouldAllowInstallPredicate
          should_allow_install);

  void set_extensions_enabled(bool enabled) { extensions_enabled_ = enabled; }
  bool extensions_enabled() { return extensions_enabled_; }

  void set_show_extensions_prompts(bool enabled) {
    show_extensions_prompts_ = enabled;
  }

  bool show_extensions_prompts() {
    return show_extensions_prompts_;
  }

  Profile* profile();

  
  
  void DestroyingProfile();

  
  
  ExtensionPrefs* extension_prefs();

  
  
  bool is_ready() { return ready_; }

  
  ExtensionUpdater* updater();

  ExtensionToolbarModel* toolbar_model() { return &toolbar_model_; }

  ExtensionsQuotaService* quota_service() { return &quota_service_; }

  ExtensionMenuManager* menu_manager() { return &menu_manager_; }

  ExtensionBrowserEventRouter* browser_event_router() {
    return browser_event_router_.get();
  }

  
  
  void ReportExtensionLoadError(const FilePath& extension_path,
                                const std::string& error,
                                NotificationType type,
                                bool be_noisy);

  
  
  void DidCreateRenderViewForBackgroundPage(ExtensionHost* host);

  
  
  void CheckExternalUninstall(const std::string& id);

  
  void ClearProvidersForTesting();

  
  
  void AddProviderForTesting(ExternalExtensionProviderInterface* test_provider);

  
  virtual void OnExternalExtensionFileFound(const std::string& id,
                                            const Version* version,
                                            const FilePath& path,
                                            Extension::Location location);

  virtual void OnExternalExtensionUpdateUrlFound(const std::string& id,
                                                 const GURL& update_url,
                                                 Extension::Location location);

  virtual void OnExternalProviderReady();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  bool HasApps() const;

  
  ExtensionIdSet GetAppIds() const;

  
  
  
  
  static void RecordPermissionMessagesHistogram(
      const Extension* e, const char* histogram);

 private:
  friend struct BrowserThread::DeleteOnThread<BrowserThread::UI>;
  friend class DeleteTask<ExtensionService>;

  
  
  struct ExtensionRuntimeData {
    
    bool background_page_ready;

    
    bool being_upgraded;

    
    PropertyBag property_bag;

    ExtensionRuntimeData();
    ~ExtensionRuntimeData();
  };
  typedef std::map<std::string, ExtensionRuntimeData> ExtensionRuntimeDataMap;

  struct NaClModuleInfo {
    NaClModuleInfo();
    ~NaClModuleInfo();

    GURL url;
    std::string mime_type;
  };
  typedef std::list<NaClModuleInfo> NaClModuleInfoList;

  virtual ~ExtensionService();

  
  void ClearExtensionData(const GURL& extension_url);

  
  
  const Extension* GetExtensionByIdInternal(const std::string& id,
                                            bool include_enabled,
                                            bool include_disabled) const;


  
  void TrackTerminatedExtension(const Extension* extension);
  void UntrackTerminatedExtension(const std::string& id);

  
  void NotifyExtensionLoaded(const Extension* extension);

  
  void NotifyExtensionUnloaded(const Extension* extension,
                               UnloadedExtensionInfo::Reason reason);

  
  void UpdateActiveExtensionsInCrashReporter();

  
  void LoadInstalledExtension(const ExtensionInfo& info, bool write_to_prefs);

  
  
  
  
  
  
  void RegisterNaClModule(const GURL& url, const std::string& mime_type);
  void UnregisterNaClModule(const GURL& url);

  
  
  void UpdatePluginListWithNaClModules();

  NaClModuleInfoList::iterator FindNaClModule(const GURL& url);

  
  Profile* profile_;

  
  ExtensionPrefs* extension_prefs_;

  
  
  ExtensionList extensions_;

  
  ExtensionList disabled_extensions_;

  
  ExtensionList terminated_extensions_;

  
  std::set<std::string> terminated_extension_ids_;

  
  PendingExtensionManager pending_extension_manager_;

  
  ExtensionRuntimeDataMap extension_runtime_data_;

  
  FilePath install_directory_;

  
  bool extensions_enabled_;

  
  bool show_extensions_prompts_;

  
  scoped_refptr<ExtensionServiceBackend> backend_;

  
  ExtensionsQuotaService quota_service_;

  
  
  bool ready_;

  
  scoped_ptr<ExtensionUpdater> updater_;

  
  ExtensionToolbarModel toolbar_model_;

  
  
  
  typedef std::map<std::string, FilePath> UnloadedExtensionPathMap;
  UnloadedExtensionPathMap unloaded_extension_paths_;

  
  
  
  typedef std::map<std::string, FilePath> DisabledExtensionPathMap;
  DisabledExtensionPathMap disabled_extension_paths_;

  
  
  typedef std::map<std::string, int> OrphanedDevTools;
  OrphanedDevTools orphaned_dev_tools_;

  NotificationRegistrar registrar_;
  PrefChangeRegistrar pref_change_registrar_;

  
  ExtensionMenuManager menu_manager_;

  
  ExtensionIconManager omnibox_icon_manager_;
  ExtensionIconManager omnibox_popup_icon_manager_;

  
  typedef std::vector<ComponentExtensionInfo> RegisteredComponentExtensions;
  RegisteredComponentExtensions component_extension_manifests_;

  
  AppsPromo apps_promo_;

  
  bool event_routers_initialized_;

  scoped_ptr<ExtensionBrowserEventRouter> browser_event_router_;

  scoped_ptr<ExtensionPreferenceEventRouter> preference_event_router_;

  
  
  
  ProviderCollection external_extension_providers_;

  
  
  
  bool external_extension_url_added_;

  NaClModuleInfoList nacl_module_list_;

  FRIEND_TEST_ALL_PREFIXES(ExtensionServiceTest,
                           InstallAppsWithUnlimtedStorage);
  FRIEND_TEST_ALL_PREFIXES(ExtensionServiceTest,
                           InstallAppsAndCheckStorageProtection);
  DISALLOW_COPY_AND_ASSIGN(ExtensionService);
};

#endif  
