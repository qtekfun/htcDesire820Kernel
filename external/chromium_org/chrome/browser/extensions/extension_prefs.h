// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_PREFS_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_PREFS_H_

#include <set>
#include <string>
#include <vector>

#include "base/memory/linked_ptr.h"
#include "base/memory/scoped_ptr.h"
#include "base/prefs/scoped_user_pref_update.h"
#include "base/time/time.h"
#include "base/values.h"
#include "chrome/browser/extensions/extension_scoped_prefs.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "extensions/browser/app_sorting.h"
#include "extensions/common/constants.h"
#include "extensions/common/extension.h"
#include "extensions/common/url_pattern_set.h"
#include "sync/api/string_ordinal.h"

class ExtensionPrefValueMap;
class PrefService;

namespace content {
class BrowserContext;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace extensions {

class AppSorting;
class ContentSettingsStore;
class ExtensionPrefsUninstallExtension;
class URLPatternSet;

class ExtensionPrefs : public ExtensionScopedPrefs,
                       public BrowserContextKeyedService {
 public:
  typedef std::vector<linked_ptr<ExtensionInfo> > ExtensionsInfo;

  
  typedef std::set<std::string> PrefKeySet;

  
  
  
  enum DelayReason {
    DELAY_REASON_NONE = 0,
    DELAY_REASON_GC = 1,
    DELAY_REASON_WAIT_FOR_IDLE = 2,
    DELAY_REASON_WAIT_FOR_IMPORTS = 3,
  };


  
  
  class TimeProvider {
   public:
    TimeProvider();

    virtual ~TimeProvider();

    
    virtual base::Time GetCurrentTime() const;

   private:
    DISALLOW_COPY_AND_ASSIGN(TimeProvider);
  };

  
  
  
  
  template <typename T, base::Value::Type type_enum_value>
  class ScopedUpdate {
   public:
    ScopedUpdate(ExtensionPrefs* prefs,
                 const std::string& extension_id,
                 const std::string& key);
    virtual ~ScopedUpdate();

    
    
    virtual T* Get();

    
    
    
    virtual T* Create();

   private:
    DISALLOW_COPY_AND_ASSIGN(ScopedUpdate);

    DictionaryPrefUpdate update_;
    const std::string extension_id_;
    const std::string key_;
  };
  typedef ScopedUpdate<base::DictionaryValue, base::Value::TYPE_DICTIONARY>
      ScopedDictionaryUpdate;
  typedef ScopedUpdate<base::ListValue, base::Value::TYPE_LIST>
      ScopedListUpdate;

  
  
  
  
  static ExtensionPrefs* Create(
      PrefService* prefs,
      const base::FilePath& root_dir,
      ExtensionPrefValueMap* extension_pref_value_map,
      scoped_ptr<AppSorting> app_sorting,
      bool extensions_disabled);

  
  
  static ExtensionPrefs* Create(
      PrefService* prefs,
      const base::FilePath& root_dir,
      ExtensionPrefValueMap* extension_pref_value_map,
      scoped_ptr<AppSorting> app_sorting,
      bool extensions_disabled,
      scoped_ptr<TimeProvider> time_provider);

  virtual ~ExtensionPrefs();

  
  static ExtensionPrefs* Get(content::BrowserContext* context);

  
  
  
  static ExtensionIdList GetExtensionsFrom(const PrefService* pref_service);

  
  
  bool IsExternalExtensionUninstalled(const std::string& id) const;

  
  
  
  bool IsExtensionDisabled(const std::string& id) const;

  
  ExtensionIdList GetToolbarOrder();
  void SetToolbarOrder(const ExtensionIdList& extension_ids);

  
  
  bool GetKnownDisabled(ExtensionIdSet* id_set_out);

  
  void SetKnownDisabled(const ExtensionIdSet& extension_ids);

  
  
  
  
  void OnExtensionInstalled(const Extension* extension,
                            Extension::State initial_state,
                            bool blacklisted_for_malware,
                            const syncer::StringOrdinal& page_ordinal);

  
  void OnExtensionUninstalled(const std::string& extension_id,
                              const Manifest::Location& location,
                              bool external_uninstall);

  
  void SetExtensionState(const std::string& extension_id, Extension::State);

  
  void GetExtensions(ExtensionIdList* out);

  
  virtual void UpdateExtensionPref(const std::string& id,
                                   const std::string& key,
                                   base::Value* value) OVERRIDE;

  virtual void DeleteExtensionPrefs(const std::string& id) OVERRIDE;

  virtual bool ReadPrefAsBoolean(const std::string& extension_id,
                                 const std::string& pref_key,
                                 bool* out_value) const OVERRIDE;

  virtual bool ReadPrefAsInteger(const std::string& extension_id,
                                 const std::string& pref_key,
                                 int* out_value) const OVERRIDE;

  virtual bool ReadPrefAsString(const std::string& extension_id,
                                const std::string& pref_key,
                                std::string* out_value) const OVERRIDE;

  virtual bool ReadPrefAsList(const std::string& extension_id,
                              const std::string& pref_key,
                              const base::ListValue** out_value) const OVERRIDE;

  virtual bool ReadPrefAsDictionary(
      const std::string& extension_id,
      const std::string& pref_key,
      const base::DictionaryValue** out_value) const OVERRIDE;

  virtual bool HasPrefForExtension(const std::string& extension_id) const
      OVERRIDE;

  
  bool DidExtensionEscalatePermissions(const std::string& id);

  
  
  void SetDidExtensionEscalatePermissions(
      const Extension* extension,
      bool did_escalate);

  
  int GetDisableReasons(const std::string& extension_id) const;
  void AddDisableReason(const std::string& extension_id,
                        Extension::DisableReason disable_reason);
  void RemoveDisableReason(const std::string& extension_id,
                           Extension::DisableReason disable_reason);
  void ClearDisableReasons(const std::string& extension_id);

  
  std::set<std::string> GetBlacklistedExtensions();

  
  void SetExtensionBlacklisted(const std::string& extension_id,
                               bool is_blacklisted);

  
  
  std::string GetVersionString(const std::string& extension_id);

  
  
  void UpdateManifest(const Extension* extension);

  
  base::FilePath GetExtensionPath(const std::string& extension_id);

  
  const base::FilePath& install_directory() const { return install_directory_; }

  
  
  
  
  
  
  bool IsExtensionBlacklisted(const std::string& id) const;

  
  
  int IncrementAcknowledgePromptCount(const std::string& extension_id);

  
  bool IsExternalExtensionAcknowledged(const std::string& extension_id);
  void AcknowledgeExternalExtension(const std::string& extension_id);

  
  bool IsBlacklistedExtensionAcknowledged(const std::string& extension_id);
  void AcknowledgeBlacklistedExtension(const std::string& extension_id);

  
  
  bool IsExternalInstallFirstRun(const std::string& extension_id);
  void SetExternalInstallFirstRun(const std::string& extension_id);

  
  
  bool HasWipeoutBeenAcknowledged(const std::string& extension_id);
  void SetWipeoutAcknowledged(const std::string& extension_id, bool value);

  
  
  
  
  
  
  bool SetAlertSystemFirstRun();

  
  
  
  bool ExtensionsBlacklistedByDefault() const;

  
  
  base::Time LastPingDay(const std::string& extension_id) const;

  
  
  void SetLastPingDay(const std::string& extension_id, const base::Time& time);

  
  base::Time BlacklistLastPingDay() const;
  void SetBlacklistLastPingDay(const base::Time& time);

  
  
  base::Time LastActivePingDay(const std::string& extension_id);
  void SetLastActivePingDay(const std::string& extension_id,
                            const base::Time& time);

  
  
  
  bool GetActiveBit(const std::string& extension_id);
  void SetActiveBit(const std::string& extension_id, bool active);

  
  
  
  PermissionSet* GetGrantedPermissions(const std::string& extension_id);

  
  
  
  void AddGrantedPermissions(const std::string& extension_id,
                             const PermissionSet* permissions);

  
  void RemoveGrantedPermissions(const std::string& extension_id,
                                const PermissionSet* permissions);

  
  
  
  PermissionSet* GetActivePermissions(const std::string& extension_id);

  
  void SetActivePermissions(const std::string& extension_id,
                            const PermissionSet* permissions);

  
  void SetExtensionRunning(const std::string& extension_id, bool is_running);

  
  
  bool IsExtensionRunning(const std::string& extension_id);

  
  
  
  void SetIsActive(const std::string& extension_id, bool is_active);
  bool IsActive(const std::string& extension_id);

  
  
  
  
  
  bool IsIncognitoEnabled(const std::string& extension_id) const;
  void SetIsIncognitoEnabled(const std::string& extension_id, bool enabled);

  
  
  
  
  
  bool AllowFileAccess(const std::string& extension_id) const;
  void SetAllowFileAccess(const std::string& extension_id, bool allow);
  bool HasAllowFileAccessSetting(const std::string& extension_id) const;

  
  
  
  
  scoped_ptr<ExtensionsInfo> GetInstalledExtensionsInfo() const;

  
  
  scoped_ptr<ExtensionsInfo> GetUninstalledExtensionsInfo() const;

  
  
  scoped_ptr<ExtensionInfo> GetInstalledExtensionInfo(
      const std::string& extension_id) const;

  
  
  void SetDelayedInstallInfo(const Extension* extension,
                             Extension::State initial_state,
                             bool blacklisted_for_malware,
                             DelayReason delay_reason,
                             const syncer::StringOrdinal& page_ordinal);

  
  
  bool RemoveDelayedInstallInfo(const std::string& extension_id);

  
  bool FinishDelayedInstallInfo(const std::string& extension_id);

  
  
  scoped_ptr<ExtensionInfo> GetDelayedInstallInfo(
      const std::string& extension_id) const;

  DelayReason GetDelayedInstallReason(const std::string& extension_id) const;

  
  
  scoped_ptr<ExtensionsInfo> GetAllDelayedInstallInfo() const;

  
  
  bool WasAppDraggedByUser(const std::string& extension_id);

  
  
  void SetAppDraggedByUser(const std::string& extension_id);

  
  
  bool HasIncognitoPrefValue(const std::string& pref_key);

  
  int GetCreationFlags(const std::string& extension_id) const;

  
  int GetDelayedInstallCreationFlags(const std::string& extension_id) const;

  
  bool IsFromWebStore(const std::string& extension_id) const;

  
  
  bool IsFromBookmark(const std::string& extension_id) const;

  
  bool WasInstalledByDefault(const std::string& extension_id) const;

  
  
  
  base::Time GetInstallTime(const std::string& extension_id) const;

  
  base::Time GetLastLaunchTime(const std::string& extension_id) const;
  void SetLastLaunchTime(const std::string& extension_id,
                         const base::Time& time);

  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  bool extensions_disabled() { return extensions_disabled_; }

  ContentSettingsStore* content_settings_store() {
    return content_settings_store_.get();
  }

  
  PrefService* pref_service() const { return prefs_; }

  
  AppSorting* app_sorting() const { return app_sorting_.get(); }

  
  
  URLPatternSet GetAllowedInstallSites();

  
  
  
  void SetNeedsStorageGarbageCollection(bool value);
  bool NeedsStorageGarbageCollection();

  
  
  const base::DictionaryValue* GetGeometryCache(
        const std::string& extension_id) const;
  void SetGeometryCache(const std::string& extension_id,
                        scoped_ptr<base::DictionaryValue> cache);

  
  
  const base::DictionaryValue* GetInstallSignature();
  void SetInstallSignature(const DictionaryValue* signature);

 private:
  friend class ExtensionPrefsBlacklistedExtensions;  
  friend class ExtensionPrefsUninstallExtension;     

  
  ExtensionPrefs(PrefService* prefs,
                 const base::FilePath& root_dir,
                 ExtensionPrefValueMap* extension_pref_value_map,
                 scoped_ptr<AppSorting> app_sorting,
                 scoped_ptr<TimeProvider> time_provider,
                 bool extensions_disabled);

  
  
  void MakePathsRelative();

  
  
  void MakePathsAbsolute(base::DictionaryValue* dict);

  
  
  
  scoped_ptr<ExtensionInfo> GetInstalledInfoHelper(
      const std::string& extension_id,
      const base::DictionaryValue* extension) const;

  
  
  bool ReadPrefAsURLPatternSet(const std::string& extension_id,
                               const std::string& pref_key,
                               URLPatternSet* result,
                               int valid_schemes);

  
  
  void SetExtensionPrefURLPatternSet(const std::string& extension_id,
                                     const std::string& pref_key,
                                     const URLPatternSet& new_value);

  
  
  bool ReadPrefAsBooleanAndReturn(const std::string& extension_id,
                                  const std::string& key) const;

  
  
  PermissionSet* ReadPrefAsPermissionSet(const std::string& extension_id,
                                         const std::string& pref_key);

  
  
  void SetExtensionPrefPermissionSet(const std::string& extension_id,
                                     const std::string& pref_key,
                                     const PermissionSet* new_value);

  
  
  const base::DictionaryValue* GetExtensionPref(const std::string& id) const;

  
  
  void FixMissingPrefs(const ExtensionIdList& extension_ids);

  
  
  void InitPrefStore();

  
  void MigratePermissions(const ExtensionIdList& extension_ids);

  
  void MigrateDisableReasons(const ExtensionIdList& extension_ids);

  
  
  bool DoesExtensionHaveState(const std::string& id,
                              Extension::State check_state) const;

  
  
  
  template <class ExtensionIdContainer>
  bool GetUserExtensionPrefIntoContainer(
      const char* pref,
      ExtensionIdContainer* id_container_out);

  
  template <class ExtensionIdContainer>
  void SetExtensionPrefFromContainer(const char* pref,
                                     const ExtensionIdContainer& strings);

  
  
  
  
  void PopulateExtensionInfoPrefs(const Extension* extension,
                                  const base::Time install_time,
                                  Extension::State initial_state,
                                  bool blacklisted_for_malware,
                                  base::DictionaryValue* extension_dict);

  
  
  
  void FinishExtensionInfoPrefs(
      const std::string& extension_id,
      const base::Time install_time,
      bool needs_sort_ordinal,
      const syncer::StringOrdinal& suggested_page_ordinal,
      base::DictionaryValue* extension_dict);

  
  PrefService* prefs_;

  
  base::FilePath install_directory_;

  
  ExtensionPrefValueMap* extension_pref_value_map_;

  
  
  scoped_ptr<AppSorting> app_sorting_;

  scoped_refptr<ContentSettingsStore> content_settings_store_;

  scoped_ptr<TimeProvider> time_provider_;

  bool extensions_disabled_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionPrefs);
};

}  

#endif  
