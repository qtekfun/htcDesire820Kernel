// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_CHROME_APP_SORTING_H_
#define CHROME_BROWSER_EXTENSIONS_CHROME_APP_SORTING_H_

#include <map>
#include <set>
#include <string>

#include "base/basictypes.h"
#include "chrome/browser/extensions/extension_prefs.h"
#include "extensions/browser/app_sorting.h"
#include "extensions/common/extension.h"
#include "sync/api/string_ordinal.h"

class ExtensionSyncService;
class PrefService;

namespace extensions {

class ExtensionScopedPrefs;

class ChromeAppSorting : public AppSorting {
 public:
  ChromeAppSorting();
  virtual ~ChromeAppSorting();

  
  virtual void SetExtensionScopedPrefs(ExtensionScopedPrefs* prefs) OVERRIDE;
  virtual void SetExtensionSyncService(
      ExtensionSyncService* extension_sync_service) OVERRIDE;
  virtual void Initialize(
      const extensions::ExtensionIdList& extension_ids) OVERRIDE;
  virtual void FixNTPOrdinalCollisions() OVERRIDE;
  virtual void EnsureValidOrdinals(
      const std::string& extension_id,
      const syncer::StringOrdinal& suggested_page) OVERRIDE;
  virtual void OnExtensionMoved(
      const std::string& moved_extension_id,
      const std::string& predecessor_extension_id,
      const std::string& successor_extension_id) OVERRIDE;
  virtual syncer::StringOrdinal GetAppLaunchOrdinal(
      const std::string& extension_id) const OVERRIDE;
  virtual void SetAppLaunchOrdinal(
      const std::string& extension_id,
      const syncer::StringOrdinal& new_app_launch_ordinal) OVERRIDE;
  virtual syncer::StringOrdinal CreateFirstAppLaunchOrdinal(
      const syncer::StringOrdinal& page_ordinal) const OVERRIDE;
  virtual syncer::StringOrdinal CreateNextAppLaunchOrdinal(
      const syncer::StringOrdinal& page_ordinal) const OVERRIDE;
  virtual syncer::StringOrdinal CreateFirstAppPageOrdinal() const OVERRIDE;
  virtual syncer::StringOrdinal GetNaturalAppPageOrdinal() const OVERRIDE;
  virtual syncer::StringOrdinal GetPageOrdinal(
      const std::string& extension_id) const OVERRIDE;
  virtual void SetPageOrdinal(
      const std::string& extension_id,
      const syncer::StringOrdinal& new_page_ordinal) OVERRIDE;
  virtual void ClearOrdinals(const std::string& extension_id) OVERRIDE;
  virtual int PageStringOrdinalAsInteger(
      const syncer::StringOrdinal& page_ordinal) const OVERRIDE;
  virtual syncer::StringOrdinal PageIntegerAsStringOrdinal(
      size_t page_index) OVERRIDE;
  virtual void MarkExtensionAsHidden(const std::string& extension_id) OVERRIDE;

 private:
  
  
  typedef std::multimap<
      syncer::StringOrdinal, std::string,
    syncer::StringOrdinal::LessThanFn> AppLaunchOrdinalMap;
  
  
  typedef std::map<
      syncer::StringOrdinal, AppLaunchOrdinalMap,
    syncer::StringOrdinal::LessThanFn> PageOrdinalMap;

  
  friend class ChromeAppSortingDefaultOrdinalsBase;
  friend class ChromeAppSortingGetMinOrMaxAppLaunchOrdinalsOnPage;
  friend class ChromeAppSortingInitializeWithNoApps;
  friend class ChromeAppSortingPageOrdinalMapping;

  
  
  enum AppLaunchOrdinalReturn {MIN_ORDINAL, MAX_ORDINAL};

  
  struct AppOrdinals {
    AppOrdinals();
    ~AppOrdinals();

    syncer::StringOrdinal page_ordinal;
    syncer::StringOrdinal app_launch_ordinal;
  };
  typedef std::map<std::string, AppOrdinals> AppOrdinalsMap;

  
  
  
  
  
  syncer::StringOrdinal GetMinOrMaxAppLaunchOrdinalsOnPage(
      const syncer::StringOrdinal& page_ordinal,
      AppLaunchOrdinalReturn return_type) const;

  
  
  void InitializePageOrdinalMap(
      const extensions::ExtensionIdList& extension_ids);

  
  void MigrateAppIndex(
      const extensions::ExtensionIdList& extension_ids);

  
  
  
  void AddOrdinalMapping(const std::string& extension_id,
                         const syncer::StringOrdinal& page_ordinal,
                         const syncer::StringOrdinal& app_launch_ordinal);

  
  void CreateOrdinalsIfNecessary(size_t minimum_size);

  
  
  
  
  void RemoveOrdinalMapping(const std::string& extension_id,
                            const syncer::StringOrdinal& page_ordinal,
                            const syncer::StringOrdinal& app_launch_ordinal);

  
  
  void SyncIfNeeded(const std::string& extension_id);

  
  void CreateDefaultOrdinals();

  
  
  
  bool GetDefaultOrdinals(const std::string& extension_id,
                          syncer::StringOrdinal* page_ordinal,
                          syncer::StringOrdinal* app_launch_ordinal);

  
  
  
  syncer::StringOrdinal ResolveCollision(
      const syncer::StringOrdinal& page_ordinal,
      const syncer::StringOrdinal& app_launch_ordinal) const;

  
  size_t CountItemsVisibleOnNtp(const AppLaunchOrdinalMap& m) const;

  ExtensionScopedPrefs* extension_scoped_prefs_;  
  ExtensionSyncService* extension_sync_service_;  

  
  
  
  
  
  
  
  PageOrdinalMap ntp_ordinal_map_;

  
  AppOrdinalsMap default_ordinals_;

  
  
  bool default_ordinals_created_;

  
  std::set<std::string> ntp_hidden_extensions_;

  DISALLOW_COPY_AND_ASSIGN(ChromeAppSorting);
};

}  

#endif  
