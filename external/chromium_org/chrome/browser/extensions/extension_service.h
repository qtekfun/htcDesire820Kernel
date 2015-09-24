// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_SERVICE_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_SERVICE_H_

#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/prefs/pref_change_registrar.h"
#include "base/strings/string16.h"
#include "chrome/browser/extensions/blacklist.h"
#include "chrome/browser/extensions/extension_function_histogram_value.h"
#include "chrome/browser/extensions/extension_prefs.h"
#include "chrome/browser/extensions/extension_sync_service.h"
#include "chrome/common/extensions/extension_constants.h"
#include "chrome/common/extensions/extension_set.h"
#include "content/public/browser/devtools_agent_host.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "extensions/browser/external_provider_interface.h"
#include "extensions/browser/management_policy.h"
#include "extensions/browser/pending_extension_manager.h"
#include "extensions/browser/process_manager.h"
#include "extensions/browser/process_map.h"
#include "extensions/browser/quota_service.h"
#include "extensions/common/extension.h"
#include "extensions/common/manifest.h"
#include "extensions/common/manifest_handlers/shared_module_info.h"
#include "extensions/common/one_shot_event.h"

class CommandLine;
class ExtensionErrorUI;
class ExtensionToolbarModel;
class GURL;
class Profile;

namespace base {
class SequencedTaskRunner;
class Version;
}

namespace extensions {
class BrowserEventRouter;
class ComponentLoader;
class ContentSettingsStore;
class CrxInstaller;
class ExtensionActionStorageManager;
class ExtensionSystem;
class ExtensionUpdater;
class PendingExtensionManager;
class SettingsFrontend;
class UpdateObserver;
}  

namespace syncer {
class SyncErrorFactory;
}

class ExtensionServiceInterface
    : public base::SupportsWeakPtr<ExtensionServiceInterface> {
 public:
  virtual ~ExtensionServiceInterface() {}
  virtual const ExtensionSet* extensions() const = 0;
  virtual const ExtensionSet* disabled_extensions() const = 0;
  virtual extensions::PendingExtensionManager* pending_extension_manager() = 0;

  
  
  virtual bool UpdateExtension(
      const std::string& id,
      const base::FilePath& path,
      const GURL& download_url,
      extensions::CrxInstaller** out_crx_installer) = 0;
  virtual const extensions::Extension* GetExtensionById(
      const std::string& id,
      bool include_disabled) const = 0;
  virtual const extensions::Extension* GetInstalledExtension(
      const std::string& id) const = 0;

  virtual const extensions::Extension* GetPendingExtensionUpdate(
      const std::string& extension_id) const = 0;
  virtual void FinishDelayedInstallation(const std::string& extension_id) = 0;

  virtual bool IsExtensionEnabled(const std::string& extension_id) const = 0;
  virtual bool IsExternalExtensionUninstalled(
      const std::string& extension_id) const = 0;

  virtual void CheckManagementPolicy() = 0;

  
  
  
  
  virtual void CheckForUpdatesSoon() = 0;

  virtual void AddExtension(const extensions::Extension* extension) = 0;
  virtual void AddComponentExtension(
      const extensions::Extension* extension) = 0;

  virtual void UnloadExtension(
      const std::string& extension_id,
      extensions::UnloadedExtensionInfo::Reason reason) = 0;
  virtual void RemoveComponentExtension(const std::string& extension_id) = 0;

  virtual bool is_ready() = 0;

  
  virtual base::SequencedTaskRunner* GetFileTaskRunner() = 0;
};

class ExtensionService
    : public ExtensionServiceInterface,
      public extensions::ExternalProviderInterface::VisitorInterface,
      public content::NotificationObserver,
      public extensions::Blacklist::Observer {
 public:
  
  
  
  const extensions::Extension* GetInstalledExtensionByUrl(
      const GURL& url) const;

  
  const extensions::Extension* GetInstalledApp(const GURL& url) const;

  
  bool IsInstalledApp(const GURL& url) const;

  
  
  static bool UninstallExtensionHelper(ExtensionService* extensions_service,
                                       const std::string& extension_id);

  
  
  ExtensionService(Profile* profile,
                   const CommandLine* command_line,
                   const base::FilePath& install_directory,
                   extensions::ExtensionPrefs* extension_prefs,
                   extensions::Blacklist* blacklist,
                   bool autoupdate_enabled,
                   bool extensions_enabled,
                   extensions::OneShotEvent* ready);

  virtual ~ExtensionService();

  
  virtual const ExtensionSet* extensions() const OVERRIDE;
  virtual const ExtensionSet* disabled_extensions() const OVERRIDE;
  const ExtensionSet* terminated_extensions() const;
  const ExtensionSet* blacklisted_extensions() const;
  const ExtensionSet* delayed_installs() const;

  
  
  scoped_ptr<ExtensionSet> GenerateInstalledExtensionsSet() const;

  
  virtual extensions::PendingExtensionManager*
      pending_extension_manager() OVERRIDE;

  const base::FilePath& install_directory() const { return install_directory_; }

  extensions::ProcessMap* process_map() { return &process_map_; }

  
  
  
  
  void OnExtensionMoved(const std::string& moved_extension_id,
                        const std::string& predecessor_extension_id,
                        const std::string& successor_extension_id);

  
  
  
  bool IsBackgroundPageReady(const extensions::Extension* extension) const;
  void SetBackgroundPageReady(const extensions::Extension* extension);

  
  
  bool IsBeingUpgraded(const extensions::Extension* extension) const;
  void SetBeingUpgraded(const extensions::Extension* extension, bool value);

  
  
  bool IsBeingReloaded(const std::string& extension_id) const;
  void SetBeingReloaded(const std::string& extension_id, bool value);

  
  
  
  bool HasUsedWebRequest(const extensions::Extension* extension) const;
  void SetHasUsedWebRequest(const extensions::Extension* extension, bool value);

  
  void Init();

  
  void MaybeBootstrapVerifier();

  
  
  
  
  void VerifyAllExtensions(bool bootstrap);

  
  
  void FinishVerifyAllExtensions(bool bootstrap, bool success);

  
  void Shutdown();

  
  
  virtual const extensions::Extension* GetExtensionById(
      const std::string& id, bool include_disabled) const OVERRIDE;

  enum IncludeFlag {
    INCLUDE_NONE        = 0,
    INCLUDE_ENABLED     = 1 << 0,
    INCLUDE_DISABLED    = 1 << 1,
    INCLUDE_TERMINATED  = 1 << 2,
    INCLUDE_BLACKLISTED = 1 << 3,
    INCLUDE_EVERYTHING = (1 << 4) - 1,
  };

  
  
  
  
  
  const extensions::Extension* GetExtensionById(const std::string& id,
                                                int include_mask) const;

  
  
  GURL GetSiteForExtensionId(const std::string& extension_id);

  
  const extensions::Extension*
      GetTerminatedExtension(const std::string& id) const;

  
  
  virtual const extensions::Extension* GetInstalledExtension(
      const std::string& id) const OVERRIDE;

  
  
  
  
  virtual bool UpdateExtension(
      const std::string& id,
      const base::FilePath& extension_path,
      const GURL& download_url,
      extensions::CrxInstaller** out_crx_installer) OVERRIDE;

  
  
  void ReloadExtension(const std::string extension_id);

  
  
  
  
  
  
  
  
  
  
  
  virtual bool UninstallExtension(std::string extension_id,
                                  bool external_uninstall,
                                  base::string16* error);

  virtual bool IsExtensionEnabled(
      const std::string& extension_id) const OVERRIDE;
  virtual bool IsExternalExtensionUninstalled(
      const std::string& extension_id) const OVERRIDE;

  
  
  virtual void EnableExtension(const std::string& extension_id);

  
  
  virtual void DisableExtension(const std::string& extension_id,
      extensions::Extension::DisableReason disable_reason);

  
  
  
  void DisableUserExtensions(const std::vector<std::string>& except_ids);

  
  
  
  void GrantPermissionsAndEnableExtension(
      const extensions::Extension* extension);

  
  
  void GrantPermissions(
      const extensions::Extension* extension);

  
  void CheckForExternalUpdates();

  
  virtual void UnloadExtension(
      const std::string& extension_id,
      extensions::UnloadedExtensionInfo::Reason reason) OVERRIDE;

  
  virtual void RemoveComponentExtension(const std::string& extension_id)
      OVERRIDE;

  
  
  void UnloadAllExtensions();

  
  void ReloadExtensions();

  
  void GarbageCollectExtensions();

  
  
  
  bool ExtensionBindingsAllowed(const GURL& url);

  
  // tab. In this case, |url| is also rewritten to an error URL.
  bool ShouldBlockUrlInBrowserTab(GURL* url);

  
  virtual void OnLoadedInstalledExtensions();

  
  
  virtual void AddExtension(const extensions::Extension* extension) OVERRIDE;

  
  
  
  virtual void AddComponentExtension(const extensions::Extension* extension)
      OVERRIDE;

  enum ImportStatus {
   IMPORT_STATUS_OK,
   IMPORT_STATUS_UNSATISFIED,
   IMPORT_STATUS_UNRECOVERABLE
  };

  
  
  ImportStatus CheckImports(
      const extensions::Extension* extension,
      std::list<extensions::SharedModuleInfo::ImportInfo>* missing_modules,
      std::list<extensions::SharedModuleInfo::ImportInfo>* outdated_modules);

  
  
  
  ImportStatus SatisfyImports(const extensions::Extension* extension);

  
  scoped_ptr<const ExtensionSet> GetDependentExtensions(
      const extensions::Extension* extension);

  
  void PruneSharedModulesOnUninstall(const extensions::Extension* extension);

  
  
  
  
  
  
  
  void OnExtensionInstalled(
      const extensions::Extension* extension,
      const syncer::StringOrdinal& page_ordinal,
      bool has_requirement_errors,
      extensions::Blacklist::BlacklistState blacklist_state,
      bool wait_for_idle);

  
  void MaybeFinishDelayedInstallations();

  
  
  void MaybeFinishDelayedInstallation(const std::string& extension_id);

  
  
  
  virtual void FinishDelayedInstallation(
     const std::string& extension_id) OVERRIDE;

  
  
  
  virtual const extensions::Extension* GetPendingExtensionUpdate(
      const std::string& extension_id) const OVERRIDE;

  
  
  
  virtual void CheckManagementPolicy() OVERRIDE;

  virtual void CheckForUpdatesSoon() OVERRIDE;

  void set_extensions_enabled(bool enabled) { extensions_enabled_ = enabled; }
  bool extensions_enabled() { return extensions_enabled_; }

  void set_show_extensions_prompts(bool enabled) {
    show_extensions_prompts_ = enabled;
  }

  bool show_extensions_prompts() {
    return show_extensions_prompts_;
  }

  Profile* profile();

  
  
  extensions::ExtensionPrefs* extension_prefs();
  const extensions::ExtensionPrefs* extension_prefs() const;

  extensions::SettingsFrontend* settings_frontend();

  void set_extension_sync_service(
      ExtensionSyncService* extension_sync_service) {
    extension_sync_service_ = extension_sync_service;
  }

  extensions::ContentSettingsStore* GetContentSettingsStore();

  
  virtual bool is_ready() OVERRIDE;

  virtual base::SequencedTaskRunner* GetFileTaskRunner() OVERRIDE;

  extensions::ComponentLoader* component_loader() {
    return component_loader_.get();
  }

  
  extensions::ExtensionUpdater* updater();

  extensions::QuotaService* quota_service() { return &quota_service_; }

  
  
  
  scoped_ptr<DictionaryValue> GetExtensionInfo(
      const std::string& extension_id) const;

  
  
  
  
  
  void ReportExtensionLoadError(const base::FilePath& extension_path,
                                const std::string& error,
                                bool be_noisy);

  
  
  void DidCreateRenderViewForBackgroundPage(extensions::ExtensionHost* host);

  
  
  void CheckExternalUninstall(const std::string& id);

  
  void SetFileTaskRunnerForTesting(base::SequencedTaskRunner* task_runner);

  
  void ClearProvidersForTesting();

  
  
  void AddProviderForTesting(
      extensions::ExternalProviderInterface* test_provider);

  
  virtual bool OnExternalExtensionFileFound(
      const std::string& id,
      const base::Version* version,
      const base::FilePath& path,
      extensions::Manifest::Location location,
      int creation_flags,
      bool mark_acknowledged) OVERRIDE;

  virtual bool OnExternalExtensionUpdateUrlFound(
      const std::string& id,
      const GURL& update_url,
      extensions::Manifest::Location location,
      int creation_flags,
      bool mark_acknowledged) OVERRIDE;

  virtual void OnExternalProviderReady(
      const extensions::ExternalProviderInterface* provider) OVERRIDE;

  
  bool AreAllExternalProvidersReady() const;

  void OnAllExternalProvidersReady();

  
  
  void IdentifyAlertableExtensions();

  
  
  
  
  
  bool PopulateExtensionErrorUI(ExtensionErrorUI* extension_error_ui);

  
  void UpdateExternalExtensionAlert();

  
  
  void AcknowledgeExternalExtension(const std::string& id);

  
  
  bool IsUnacknowledgedExternalExtension(
      const extensions::Extension* extension);

  
  void ReconcileKnownDisabled();

  
  
  void HandleExtensionAlertDetails();

  
  
  void HandleExtensionAlertClosed();

  
  
  void HandleExtensionAlertAccept();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  bool HasApps() const;

  
  extensions::ExtensionIdSet GetAppIds() const;

  
  
  
  
  static void RecordPermissionMessagesHistogram(
      const extensions::Extension* e, const char* histogram);

#if defined(UNIT_TEST)
  void TrackTerminatedExtensionForTest(const extensions::Extension* extension) {
    TrackTerminatedExtension(extension);
  }

  void FinishInstallationForTest(const extensions::Extension* extension) {
    FinishInstallation(extension);
  }
#endif

  base::WeakPtr<ExtensionService> AsWeakPtr() { return base::AsWeakPtr(this); }

  bool browser_terminating() const { return browser_terminating_; }

  
  void set_browser_terminating_for_test(bool value) {
    browser_terminating_ = value;
  }

  
  
  
  void set_install_updates_when_idle_for_test(bool value) {
    install_updates_when_idle_ = value;
  }

  
  
  void set_external_updates_finished_callback_for_test(
      const base::Closure& callback) {
    external_updates_finished_callback_ = callback;
  }

  
  void AddUpdateObserver(extensions::UpdateObserver* observer);
  void RemoveUpdateObserver(extensions::UpdateObserver* observer);

#if defined(OS_CHROMEOS)
  void disable_garbage_collection() {
    disable_garbage_collection_ = true;
  }
  void enable_garbage_collection() {
    disable_garbage_collection_ = false;
  }
#endif

 private:
  
  
  struct ExtensionRuntimeData {
    
    bool background_page_ready;

    
    bool being_upgraded;

    
    bool has_used_webrequest;

    ExtensionRuntimeData();
    ~ExtensionRuntimeData();
  };
  typedef std::map<std::string, ExtensionRuntimeData> ExtensionRuntimeDataMap;

  
  void SetReadyAndNotifyListeners();

  
  void OnExtensionInstallPrefChanged();

  
  
  void TrackTerminatedExtension(const extensions::Extension* extension);

  
  
  void UntrackTerminatedExtension(const std::string& id);

  
  
  
  
  void AddNewOrUpdatedExtension(
      const extensions::Extension* extension,
      extensions::Extension::State initial_state,
      extensions::Blacklist::BlacklistState blacklist_state,
      const syncer::StringOrdinal& page_ordinal);

  
  void NotifyExtensionLoaded(const extensions::Extension* extension);

  
  void NotifyExtensionUnloaded(
      const extensions::Extension* extension,
      extensions::UnloadedExtensionInfo::Reason reason);

  
  void FinishInstallation(const extensions::Extension* extension);

  
  
  
  void UpdateActivePermissions(const extensions::Extension* extension);

  
  
  void CheckPermissionsIncrease(const extensions::Extension* extension,
                                bool is_extension_installed);

  
  void UpdateActiveExtensionsInCrashReporter();

  
  
  bool ShouldEnableOnInstall(const extensions::Extension* extension);

  
  
  bool IsExtensionIdle(const std::string& extension_id) const;

  
  
  bool ShouldDelayExtensionUpdate(const std::string& extension_id,
                                  bool wait_for_idle) const;

  
  
  void GarbageCollectIsolatedStorage();
  void OnGarbageCollectIsolatedStorageFinished();
  void OnNeedsToGarbageCollectIsolatedStorage();

  
  virtual void OnBlacklistUpdated() OVERRIDE;

  
  
  void ManageBlacklist(const std::set<std::string>& blacklisted_ids);

  
  
  void set_installs_delayed_for_gc(bool value) {
    installs_delayed_for_gc_ = value;
  }
  bool installs_delayed_for_gc() const { return installs_delayed_for_gc_; }

  
  Profile* profile_;

  
  extensions::ExtensionSystem* system_;

  
  extensions::ExtensionPrefs* extension_prefs_;

  
  extensions::Blacklist* blacklist_;

  
  scoped_ptr<extensions::SettingsFrontend> settings_frontend_;

  
  ExtensionSyncService* extension_sync_service_;

  
  ExtensionSet extensions_;

  
  ExtensionSet disabled_extensions_;

  
  ExtensionSet terminated_extensions_;

  
  
  
  
  ExtensionSet blacklisted_extensions_;

  
  
  ExtensionSet delayed_installs_;

  
  extensions::PendingExtensionManager pending_extension_manager_;

  
  ExtensionRuntimeDataMap extension_runtime_data_;

  
  base::FilePath install_directory_;

  
  bool extensions_enabled_;

  
  bool show_extensions_prompts_;

  
  
  bool install_updates_when_idle_;

  
  extensions::QuotaService quota_service_;

  
  extensions::OneShotEvent* const ready_;

  
  scoped_ptr<extensions::ExtensionUpdater> updater_;

  
  
  
  typedef std::map<std::string, base::FilePath> UnloadedExtensionPathMap;
  UnloadedExtensionPathMap unloaded_extension_paths_;

  
  std::set<std::string> reloading_extensions_;

  
  
  typedef std::map<std::string, scoped_refptr<content::DevToolsAgentHost> >
      OrphanedDevTools;
  OrphanedDevTools orphaned_dev_tools_;

  content::NotificationRegistrar registrar_;
  PrefChangeRegistrar pref_change_registrar_;

  
  scoped_ptr<extensions::ComponentLoader> component_loader_;

  
  
  
  extensions::ProviderCollection external_extension_providers_;

  
  
  
  
  
  bool update_once_all_providers_are_ready_;

  
  
  
  base::Closure external_updates_finished_callback_;

  
  
  
  bool browser_terminating_;

  
  
  
  
  bool installs_delayed_for_gc_;

  
  
  
  bool is_first_run_;

  extensions::ProcessMap process_map_;

  
  
  std::set<std::string> extensions_being_reloaded_;

  scoped_ptr<ExtensionErrorUI> extension_error_ui_;
  
  scoped_refptr<base::SequencedTaskRunner> file_task_runner_;

#if defined(ENABLE_EXTENSIONS)
  scoped_ptr<extensions::ExtensionActionStorageManager>
      extension_action_storage_manager_;
#endif
  scoped_ptr<extensions::ManagementPolicy::Provider>
      shared_module_policy_provider_;

  ObserverList<extensions::UpdateObserver, true> update_observers_;

#if defined(OS_CHROMEOS)
  
  
  
  
  bool disable_garbage_collection_;
#endif

  FRIEND_TEST_ALL_PREFIXES(ExtensionServiceTest,
                           InstallAppsWithUnlimtedStorage);
  FRIEND_TEST_ALL_PREFIXES(ExtensionServiceTest,
                           InstallAppsAndCheckStorageProtection);
  DISALLOW_COPY_AND_ASSIGN(ExtensionService);
};

#endif  
