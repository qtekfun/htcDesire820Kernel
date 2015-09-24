// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_STORAGE_SETTINGS_SYNC_PROCESSOR_H_
#define CHROME_BROWSER_EXTENSIONS_API_STORAGE_SETTINGS_SYNC_PROCESSOR_H_

#include <set>
#include <string>

#include "chrome/browser/value_store/value_store_change.h"
#include "sync/api/sync_error.h"

namespace syncer {
class SyncChangeProcessor;
}  

namespace extensions {

class SettingsSyncProcessor {
 public:
  SettingsSyncProcessor(const std::string& extension_id,
                        syncer::ModelType type,
                        syncer::SyncChangeProcessor* sync_processor);
  ~SettingsSyncProcessor();

  
  void Init(const base::DictionaryValue& initial_state);

  
  syncer::SyncError SendChanges(const ValueStoreChangeList& changes);

  
  
  void NotifyChanges(const ValueStoreChangeList& changes);

  syncer::ModelType type() { return type_; }

 private:
  
  const std::string extension_id_;

  
  const syncer::ModelType type_;

  
  syncer::SyncChangeProcessor* const sync_processor_;

  
  bool initialized_;

  
  
  std::set<std::string> synced_keys_;

  DISALLOW_COPY_AND_ASSIGN(SettingsSyncProcessor);
};

}  

#endif  
