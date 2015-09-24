// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_DOM_DISTILLER_CORE_DOM_DISTILLER_MODEL_H_
#define COMPONENTS_DOM_DISTILLER_CORE_DOM_DISTILLER_MODEL_H_

#include <string>
#include <vector>

#include "base/containers/hash_tables.h"
#include "base/id_map.h"
#include "components/dom_distiller/core/article_entry.h"
#include "sync/api/sync_change.h"
#include "sync/api/sync_change_processor.h"  
#include "sync/api/sync_data.h"
#include "url/gurl.h"

namespace dom_distiller {

class DomDistillerModel {
 public:
  DomDistillerModel();
  explicit DomDistillerModel(const std::vector<ArticleEntry>& initial_data);

  ~DomDistillerModel();

  
  
  bool GetEntryById(const std::string& entry_id, ArticleEntry* entry) const;
  bool GetEntryByUrl(const GURL& url, ArticleEntry* entry) const;

  std::vector<ArticleEntry> GetEntries() const;
  size_t GetNumEntries() const;

  syncer::SyncDataList GetAllSyncData() const;

  
  
  
  void CalculateChangesForMerge(const syncer::SyncDataList& data,
                                syncer::SyncChangeList* changes_to_apply,
                                syncer::SyncChangeList* changes_missing);

  
  
  
  
  
  
  void ApplyChangesToModel(const syncer::SyncChangeList& change_list,
                           syncer::SyncChangeList* changes_applied,
                           syncer::SyncChangeList* changes_missing);

 private:
  typedef int32 KeyType;
  typedef base::hash_map<KeyType, ArticleEntry> EntryMap;
  typedef base::hash_map<std::string, KeyType> StringToKeyMap;

  void AddEntry(const ArticleEntry& entry);
  void RemoveEntry(const ArticleEntry& entry);

  
  
  bool GetKeyById(const std::string& entry_id, KeyType* key) const;
  bool GetKeyByUrl(const GURL& url, KeyType* key) const;

  
  
  void GetEntryByKey(KeyType key, ArticleEntry* entry) const;

  void ApplyChangeToModel(const syncer::SyncChange& change,
                          syncer::SyncChangeList* changes_applied,
                          syncer::SyncChangeList* changes_missing);

  KeyType next_key_;
  EntryMap entries_;
  StringToKeyMap url_to_key_map_;
  StringToKeyMap entry_id_to_key_map_;

  DISALLOW_COPY_AND_ASSIGN(DomDistillerModel);
};

}  

#endif  
