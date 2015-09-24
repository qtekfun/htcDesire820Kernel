// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_PREFS_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_PREFS_H_
#pragma once

#include <set>
#include <string>
#include <vector>

#include "base/memory/linked_ptr.h"
#include "base/time.h"
#include "chrome/browser/prefs/pref_service.h"
#include "chrome/common/extensions/extension.h"
#include "googleurl/src/gurl.h"

class ExtensionPrefValueMap;

class ExtensionPrefs {
 public:
  
  
  
  static const char kExtensionsPref[];

  typedef std::vector<linked_ptr<ExtensionInfo> > ExtensionsInfo;

  
  typedef std::set<std::string> PrefKeySet;

  
  typedef std::vector<std::string> ExtensionIdSet;

  
  
  
  
  enum LaunchType {
    LAUNCH_PINNED,
    LAUNCH_REGULAR,
    LAUNCH_FULLSCREEN,
    LAUNCH_WINDOW,

    
    
    
    LAUNCH_DEFAULT = LAUNCH_REGULAR
  };

  
  explicit ExtensionPrefs(PrefService* prefs,
                          const FilePath& root_dir,
                          ExtensionPrefValueMap* extension_pref_value_map);
  virtual ~ExtensionPrefs();

  
  
  
  DictionaryValue* CopyCurrentExtensions();

  
  
  bool IsExternalExtensionUninstalled(const std::string& id) const;

  
  typedef std::vector<GURL> URLList;
  URLList GetShelfToolstripOrder();

  
  void SetShelfToolstripOrder(const URLList& urls);

  
  std::vector<std::string> GetToolbarOrder();

  
  void SetToolbarOrder(const std::vector<std::string>& extension_ids);

  
  void OnExtensionInstalled(const Extension* extension,
                            Extension::State initial_state,
                            bool initial_incognito_enabled);

  
  void OnExtensionUninstalled(const std::string& extension_id,
                              const Extension::Location& location,
                              bool external_uninstall);

  
  Extension::State GetExtensionState(const std::string& extension_id) const;

  
  void SetExtensionState(const Extension* extension, Extension::State);

  
  void GetExtensions(ExtensionIdSet* out);

  
  bool GetBrowserActionVisibility(const Extension* extension);
  void SetBrowserActionVisibility(const Extension* extension, bool visible);

  
  bool DidExtensionEscalatePermissions(const std::string& id);

  
  
  void SetDidExtensionEscalatePermissions(const Extension* extension,
                                          bool did_escalate);

  
  
  std::string GetVersionString(const std::string& extension_id);

  
  
  void UpdateManifest(const Extension* extension);

  
  FilePath GetExtensionPath(const std::string& extension_id);

  
  const FilePath& install_directory() const { return install_directory_; }

  
  void UpdateBlacklist(const std::set<std::string>& blacklist_set);

  
  bool IsExtensionBlacklisted(const std::string& id);

  
  
  bool IsExtensionAllowedByPolicy(const std::string& extension_id);

  
  
  base::Time LastPingDay(const std::string& extension_id) const;

  
  
  void SetLastPingDay(const std::string& extension_id, const base::Time& time);

  
  base::Time BlacklistLastPingDay() const;
  void SetBlacklistLastPingDay(const base::Time& time);

  
  
  base::Time LastActivePingDay(const std::string& extension_id);
  void SetLastActivePingDay(const std::string& extension_id,
                            const base::Time& time);

  
  
  
  bool GetActiveBit(const std::string& extension_id);
  void SetActiveBit(const std::string& extension_id, bool active);

  
  
  
  
  
  
  
  bool GetGrantedPermissions(const std::string& extension_id,
                             bool* full_access,
                             std::set<std::string>* api_permissions,
                             ExtensionExtent* host_extent);

  
  
  
  
  void AddGrantedPermissions(const std::string& extension_id,
                             const bool full_access,
                             const std::set<std::string>& api_permissions,
                             const ExtensionExtent& host_extent);

  
  
  bool IsIncognitoEnabled(const std::string& extension_id);
  void SetIsIncognitoEnabled(const std::string& extension_id, bool enabled);

  
  
  bool AllowFileAccess(const std::string& extension_id);
  void SetAllowFileAccess(const std::string& extension_id, bool allow);
  bool HasAllowFileAccessSetting(const std::string& extension_id) const;

  
  
  LaunchType GetLaunchType(const std::string& extension_id,
                           LaunchType default_pref_value);

  void SetLaunchType(const std::string& extension_id, LaunchType launch_type);

  
  
  
  extension_misc::LaunchContainer GetLaunchContainer(
      const Extension* extension,
      LaunchType default_pref_value);


  
  
  
  
  ExtensionsInfo* GetInstalledExtensionsInfo();

  
  
  ExtensionInfo* GetInstalledExtensionInfo(const std::string& extension_id);

  
  
  void SetIdleInstallInfo(const std::string& extension_id,
                          const FilePath& crx_path,
                          const std::string& version,
                          const base::Time& fetch_time);

  
  
  bool RemoveIdleInstallInfo(const std::string& extension_id);

  
  
  bool GetIdleInstallInfo(const std::string& extension_id,
                          FilePath* crx_path,
                          std::string* version,
                          base::Time* fetch_time);

  
  std::set<std::string> GetIdleInstallInfoIds();

  
  
  
  
  
  bool GetWebStoreLogin(std::string* result);
  void SetWebStoreLogin(const std::string& login);

  
  
  
  
  int GetAppLaunchIndex(const std::string& extension_id);

  
  void SetAppLaunchIndex(const std::string& extension_id, int index);

  
  
  int GetNextAppLaunchIndex();

  
  void SetAppLauncherOrder(const std::vector<std::string>& extension_ids);

  
  
  
  int GetPageIndex(const std::string& extension_id);

  
  void SetPageIndex(const std::string& extension_id, int index);

  
  
  bool WasAppDraggedByUser(const std::string& extension_id);

  
  
  void SetAppDraggedByUser(const std::string& extension_id);

  
  
  
  void SetUpdateUrlData(const std::string& extension_id,
                        const std::string& data);
  std::string GetUpdateUrlData(const std::string& extension_id);

  
  
  
  
  void SetExtensionControlledPref(const std::string& extension_id,
                                  const std::string& pref_key,
                                  bool incognito,
                                  Value* value);

  void RemoveExtensionControlledPref(const std::string& extension_id,
                                     const std::string& pref_key,
                                     bool incognito);

  
  
  bool CanExtensionControlPref(const std::string& extension_id,
                               const std::string& pref_key,
                               bool incognito);

  
  
  bool DoesExtensionControlPref(const std::string& extension_id,
                                const std::string& pref_key,
                                bool incognito);

  
  
  bool HasIncognitoPrefValue(const std::string& pref_key);

  static void RegisterUserPrefs(PrefService* prefs);

  
  PrefService* pref_service() const { return prefs_; }

 protected:
  
  
  virtual base::Time GetCurrentTime() const;

 private:
  
  
  void MakePathsRelative();

  
  
  void MakePathsAbsolute(DictionaryValue* dict);

  
  void UpdateExtensionPref(const std::string& id,
                           const std::string& key,
                           Value* value);

  
  void DeleteExtensionPrefs(const std::string& id);

  
  
  bool ReadBooleanFromPref(const DictionaryValue* ext,
                           const std::string& pref_key);

  
  bool ReadExtensionPrefBoolean(const std::string& extension_id,
                                const std::string& pref_key);

  
  
  bool ReadIntegerFromPref(const DictionaryValue* ext,
                           const std::string& pref_key,
                           int* out_value);

  
  bool ReadExtensionPrefInteger(const std::string& extension_id,
                                const std::string& pref_key,
                                int* out_value);

  
  bool ReadExtensionPrefList(const std::string& extension_id,
                             const std::string& pref_key,
                             const ListValue** out_value);

  
  
  bool ReadExtensionPrefStringSet(const std::string& extension_id,
                                  const std::string& pref_key,
                                  std::set<std::string>* result);

  
  
  
  void AddToExtensionPrefStringSet(const std::string& extension_id,
                                   const std::string& pref_key,
                                   const std::set<std::string>& added_values);

  
  
  const DictionaryValue* GetExtensionPref(const std::string& id) const;

  
  
  
  const DictionaryValue* GetExtensionControlledPrefs(
      const std::string& id) const;

  
  
  
  
  
  
  void SavePrefs();

  
  
  
  bool IsBlacklistBitSet(DictionaryValue* ext);

  
  
  
  base::Time GetInstallTime(const std::string& extension_id) const;

  
  
  void FixMissingPrefs(const ExtensionIdSet& extension_ids);

  
  
  void InitPrefStore();

  
  PrefService* prefs_;

  
  FilePath install_directory_;

  
  ExtensionPrefValueMap* extension_pref_value_map_;

  
  URLList shelf_order_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionPrefs);
};

#endif  
