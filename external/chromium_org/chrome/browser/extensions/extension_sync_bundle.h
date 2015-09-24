// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_SYNC_BUNDLE_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_SYNC_BUNDLE_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/extensions/extension_sync_data.h"
#include "chrome/browser/extensions/sync_bundle.h"
#include "sync/api/syncable_service.h"

class ExtensionSyncService;
class ExtensionSet;

namespace syncer {
class SyncChangeProcessor;
class SyncErrorFactory;
}

namespace extensions {

class Extension;

class ExtensionSyncBundle : public SyncBundle {
 public:
  explicit ExtensionSyncBundle(ExtensionSyncService* extension_sync_service);
  virtual ~ExtensionSyncBundle();

  
  void SetupSync(syncer::SyncChangeProcessor* sync_processor,
                 syncer::SyncErrorFactory* sync_error_factory,
                 const syncer::SyncDataList& initial_sync_data);

  
  
  void Reset();

  
  syncer::SyncChange CreateSyncChangeToDelete(const Extension* extension) const;

  
  void ProcessDeletion(
      std::string extension_id, const syncer::SyncChange& sync_change);

  
  syncer::SyncChange CreateSyncChange(const syncer::SyncData& sync_data);

  
  syncer::SyncDataList GetAllSyncData() const;

  
  void ProcessSyncChange(ExtensionSyncData extension_sync_data);

  
  void ProcessSyncChangeList(syncer::SyncChangeList sync_change_list);

  
  bool HasExtensionId(const std::string& id) const;
  bool HasPendingExtensionId(const std::string& id) const;

  
  void AddPendingExtension(const std::string& id,
                           const ExtensionSyncData& extension_sync_data);

  
  std::vector<ExtensionSyncData> GetPendingData() const;

  
  void GetExtensionSyncDataListHelper(
      const ExtensionSet* extensions,
      std::vector<extensions::ExtensionSyncData>* sync_data_list) const;

  
  
  virtual bool IsSyncing() const OVERRIDE;

  
  virtual void SyncChangeIfNeeded(const Extension& extension) OVERRIDE;

 private:
  
  void AddExtension(const std::string& id);

  
  void RemoveExtension(const std::string& id);

  
  void MarkPendingExtensionSynced(const std::string& id);

  ExtensionSyncService* extension_sync_service_;  
  scoped_ptr<syncer::SyncChangeProcessor> sync_processor_;
  scoped_ptr<syncer::SyncErrorFactory> sync_error_factory_;

  std::set<std::string> synced_extensions_;
  std::map<std::string, ExtensionSyncData> pending_sync_data_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionSyncBundle);
};

}  

#endif  
