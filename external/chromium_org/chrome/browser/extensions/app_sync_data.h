// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_APP_SYNC_DATA_H_
#define CHROME_BROWSER_EXTENSIONS_APP_SYNC_DATA_H_

#include "chrome/browser/extensions/extension_sync_data.h"
#include "sync/api/string_ordinal.h"
#include "sync/api/sync_change.h"

namespace syncer {
class SyncData;
}

namespace sync_pb {
class AppSpecifics;
}

namespace extensions {

class Extension;
class ExtensionSyncData;

class AppSyncData {
 public:
  AppSyncData();
  explicit AppSyncData(const syncer::SyncData& sync_data);
  explicit AppSyncData(const syncer::SyncChange& sync_change);
  AppSyncData(const Extension& extension,
              bool enabled,
              bool incognito_enabled,
              const syncer::StringOrdinal& app_launch_ordinal,
              const syncer::StringOrdinal& page_ordinal);
  ~AppSyncData();

  
  syncer::SyncData GetSyncData() const;
  syncer::SyncChange GetSyncChange(
      syncer::SyncChange::SyncChangeType change_type) const;

  const std::string& id() const { return extension_sync_data_.id(); }

  bool uninstalled() const { return extension_sync_data_.uninstalled(); }

  
  
  const syncer::StringOrdinal& app_launch_ordinal() const {
    return app_launch_ordinal_;
  }
  const syncer::StringOrdinal& page_ordinal() const { return page_ordinal_; }

  const ExtensionSyncData& extension_sync_data() const {
    return extension_sync_data_;
  }

 private:
  
  void PopulateAppSpecifics(sync_pb::AppSpecifics* specifics) const;

  
  void PopulateFromAppSpecifics(
      const sync_pb::AppSpecifics& specifics);
  void PopulateFromSyncData(const syncer::SyncData& sync_data);

  ExtensionSyncData extension_sync_data_;
  syncer::StringOrdinal app_launch_ordinal_;
  syncer::StringOrdinal page_ordinal_;
};

}  

#endif  

