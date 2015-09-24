// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_PENDING_ENABLES_H_
#define CHROME_BROWSER_EXTENSIONS_PENDING_ENABLES_H_

#include <set>

#include "sync/api/syncable_service.h"

class ExtensionService;
class ProfileSyncService;

namespace browser_sync {
class SyncPrefs;
}

namespace extensions {

class SyncBundle;

class PendingEnables {
 public:
  PendingEnables(scoped_ptr<browser_sync::SyncPrefs> sync_prefs,
                 SyncBundle* sync_bundle,
                 syncer::ModelType enable_type);
  ~PendingEnables();

  
  
  
  void OnExtensionEnabled(const std::string& extension_id);
  void OnExtensionDisabled(const std::string& extension_id);

  
  
  void OnSyncStarted(ExtensionService* service);

  
  bool Contains(const std::string& extension_id) const;

 private:
  bool IsSyncEnabled();
  bool IsWaitingForSync();

  scoped_ptr<browser_sync::SyncPrefs> sync_prefs_;
  SyncBundle* sync_bundle_;
  syncer::ModelType enable_type_;
  std::set<std::string> ids_;

  bool is_sync_enabled_for_test_;

  DISALLOW_COPY_AND_ASSIGN(PendingEnables);
};

}  

#endif  
