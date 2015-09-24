// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_SYNC_DATA_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_SYNC_DATA_H_

#include <string>

#include "base/version.h"
#include "sync/api/sync_change.h"
#include "url/gurl.h"

namespace syncer {
class SyncData;
}

namespace sync_pb {
class ExtensionSpecifics;
}

namespace extensions {

class Extension;

class ExtensionSyncData {
 public:
  ExtensionSyncData();
  explicit ExtensionSyncData(const syncer::SyncData& sync_data);
  explicit ExtensionSyncData(const syncer::SyncChange& sync_change);
  ExtensionSyncData(const Extension& extension,
                    bool enabled,
                    bool incognito_enabled);
  ~ExtensionSyncData();

  
  syncer::SyncData GetSyncData() const;
  syncer::SyncChange GetSyncChange(
      syncer::SyncChange::SyncChangeType change_type) const;

  
  void PopulateExtensionSpecifics(sync_pb::ExtensionSpecifics* specifics) const;

  
  void PopulateFromExtensionSpecifics(
      const sync_pb::ExtensionSpecifics& specifics);

  void set_uninstalled(bool uninstalled);

  const std::string& id() const { return id_; }

  
  
  
  bool uninstalled() const { return uninstalled_; }
  bool enabled() const { return enabled_; }
  bool incognito_enabled() const { return incognito_enabled_; }

  
  
  const Version& version() const { return version_; }
  const GURL& update_url() const { return update_url_; }
  
  const std::string& name() const { return name_; }

 private:
  
  void PopulateFromSyncData(const syncer::SyncData& sync_data);

  std::string id_;
  bool uninstalled_;
  bool enabled_;
  bool incognito_enabled_;
  Version version_;
  GURL update_url_;
  std::string name_;
};

}  

#endif  
