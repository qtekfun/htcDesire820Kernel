// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_DOM_DISTILLER_CORE_DOM_DISTILLER_STORE_H_
#define COMPONENTS_DOM_DISTILLER_CORE_DOM_DISTILLER_STORE_H_

#include <string>
#include <vector>

#include "base/containers/hash_tables.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "components/dom_distiller/core/article_entry.h"
#include "components/dom_distiller/core/dom_distiller_database.h"
#include "components/dom_distiller/core/dom_distiller_model.h"
#include "components/dom_distiller/core/dom_distiller_observer.h"
#include "sync/api/sync_change.h"
#include "sync/api/sync_data.h"
#include "sync/api/sync_error.h"
#include "sync/api/sync_error_factory.h"
#include "sync/api/sync_merge_result.h"
#include "sync/api/syncable_service.h"
#include "url/gurl.h"

namespace base {
class FilePath;
}

namespace dom_distiller {

class DomDistillerStoreInterface {
 public:
  virtual ~DomDistillerStoreInterface() {}

  
  virtual syncer::SyncableService* GetSyncableService() = 0;

  virtual bool AddEntry(const ArticleEntry& entry) = 0;

  
  virtual bool UpdateEntry(const ArticleEntry& entry) = 0;

  virtual bool RemoveEntry(const ArticleEntry& entry) = 0;

  
  
  virtual bool GetEntryById(const std::string& entry_id,
                            ArticleEntry* entry) = 0;
  virtual bool GetEntryByUrl(const GURL& url, ArticleEntry* entry) = 0;

  
  virtual std::vector<ArticleEntry> GetEntries() const = 0;

  virtual void AddObserver(DomDistillerObserver* observer) = 0;

  virtual void RemoveObserver(DomDistillerObserver* observer) = 0;
};

class DomDistillerStore : public syncer::SyncableService,
                          public DomDistillerStoreInterface {
 public:
  
  
  DomDistillerStore(scoped_ptr<DomDistillerDatabaseInterface> database,
                    const base::FilePath& database_dir);

  
  
  
  DomDistillerStore(scoped_ptr<DomDistillerDatabaseInterface> database,
                    const std::vector<ArticleEntry>& initial_data,
                    const base::FilePath& database_dir);

  virtual ~DomDistillerStore();

  
  virtual syncer::SyncableService* GetSyncableService() OVERRIDE;
  virtual bool AddEntry(const ArticleEntry& entry) OVERRIDE;
  virtual bool UpdateEntry(const ArticleEntry& entry) OVERRIDE;
  virtual bool RemoveEntry(const ArticleEntry& entry) OVERRIDE;
  virtual bool GetEntryById(const std::string& entry_id,
                            ArticleEntry* entry) OVERRIDE;
  virtual bool GetEntryByUrl(const GURL& url, ArticleEntry* entry) OVERRIDE;
  virtual std::vector<ArticleEntry> GetEntries() const OVERRIDE;
  virtual void AddObserver(DomDistillerObserver* observer) OVERRIDE;
  virtual void RemoveObserver(DomDistillerObserver* observer) OVERRIDE;

  
  virtual syncer::SyncMergeResult MergeDataAndStartSyncing(
      syncer::ModelType type,
      const syncer::SyncDataList& initial_sync_data,
      scoped_ptr<syncer::SyncChangeProcessor> sync_processor,
      scoped_ptr<syncer::SyncErrorFactory> error_handler) OVERRIDE;
  virtual void StopSyncing(syncer::ModelType type) OVERRIDE;
  virtual syncer::SyncDataList GetAllSyncData(syncer::ModelType type) const
      OVERRIDE;
  virtual syncer::SyncError ProcessSyncChanges(
      const tracked_objects::Location& from_here,
      const syncer::SyncChangeList& change_list) OVERRIDE;
 private:
  void OnDatabaseInit(bool success);
  void OnDatabaseLoad(bool success, scoped_ptr<EntryVector> entries);
  void OnDatabaseSave(bool success);

  syncer::SyncMergeResult MergeDataWithModel(
      const syncer::SyncDataList& data,
      syncer::SyncChangeList* changes_applied,
      syncer::SyncChangeList* changes_missing);

  
  
  
  void CalculateChangesForMerge(const syncer::SyncDataList& data,
                                syncer::SyncChangeList* changes_to_apply,
                                syncer::SyncChangeList* changes_missing);

  bool ApplyChangesToSync(const tracked_objects::Location& from_here,
                          const syncer::SyncChangeList& change_list);
  bool ApplyChangesToDatabase(const syncer::SyncChangeList& change_list);

  
  
  void ApplyChangesToModel(const syncer::SyncChangeList& change_list,
                           syncer::SyncChangeList* changes_applied,
                           syncer::SyncChangeList* changes_missing);

  void NotifyObservers(const syncer::SyncChangeList& changes);

  scoped_ptr<syncer::SyncChangeProcessor> sync_processor_;
  scoped_ptr<syncer::SyncErrorFactory> error_factory_;
  scoped_ptr<DomDistillerDatabaseInterface> database_;
  bool database_loaded_;
  ObserverList<DomDistillerObserver> observers_;

  DomDistillerModel model_;

  base::WeakPtrFactory<DomDistillerStore> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(DomDistillerStore);
};

}  

#endif  
