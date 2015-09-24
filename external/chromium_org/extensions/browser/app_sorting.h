// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_BROWSER_APP_SORTING_H_
#define EXTENSIONS_BROWSER_APP_SORTING_H_

#include <string>

#include "base/basictypes.h"
#include "extensions/common/extension.h"
#include "sync/api/string_ordinal.h"

class ExtensionSyncService;

namespace extensions {

class ExtensionScopedPrefs;

class AppSorting {
 public:
  AppSorting() {}
  virtual ~AppSorting() {}

  
  
  virtual void SetExtensionScopedPrefs(ExtensionScopedPrefs* prefs) = 0;

  
  virtual void SetExtensionSyncService(
      ExtensionSyncService* extension_sync_service) = 0;

  
  virtual void Initialize(const extensions::ExtensionIdList& extension_ids) = 0;

  
  
  
  virtual void FixNTPOrdinalCollisions() = 0;

  
  
  
  virtual void EnsureValidOrdinals(
      const std::string& extension_id,
      const syncer::StringOrdinal& suggested_page) = 0;

  
  
  
  virtual void OnExtensionMoved(const std::string& moved_extension_id,
                                const std::string& predecessor_extension_id,
                                const std::string& successor_extension_id) = 0;

  
  
  
  
  
  
  virtual syncer::StringOrdinal GetAppLaunchOrdinal(
      const std::string& extension_id) const = 0;

  
  virtual void SetAppLaunchOrdinal(
      const std::string& extension_id,
      const syncer::StringOrdinal& new_app_launch_ordinal) = 0;

  
  
  virtual syncer::StringOrdinal CreateFirstAppLaunchOrdinal(
      const syncer::StringOrdinal& page_ordinal) const = 0;

  
  
  virtual syncer::StringOrdinal CreateNextAppLaunchOrdinal(
      const syncer::StringOrdinal& page_ordinal) const = 0;

  
  virtual syncer::StringOrdinal CreateFirstAppPageOrdinal() const = 0;

  
  
  
  virtual syncer::StringOrdinal GetNaturalAppPageOrdinal() const = 0;

  
  
  
  virtual syncer::StringOrdinal GetPageOrdinal(
      const std::string& extension_id) const = 0;

  
  virtual void SetPageOrdinal(
      const std::string& extension_id,
      const syncer::StringOrdinal& new_page_ordinal) = 0;

  
  virtual void ClearOrdinals(const std::string& extension_id) = 0;

  
  
  virtual int PageStringOrdinalAsInteger(
      const syncer::StringOrdinal& page_ordinal) const = 0;

  
  
  virtual syncer::StringOrdinal PageIntegerAsStringOrdinal(
      size_t page_index) = 0;

  
  virtual void MarkExtensionAsHidden(const std::string& extension_id) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(AppSorting);
};

}  

#endif  
