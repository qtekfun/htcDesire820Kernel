// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_STORAGE_SETTING_SYNC_DATA_H_
#define CHROME_BROWSER_EXTENSIONS_API_STORAGE_SETTING_SYNC_DATA_H_

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/values.h"
#include "sync/api/sync_change.h"

namespace syncer {
class SyncData;
}

namespace sync_pb {
class ExtensionSettingSpecifics;
}

namespace extensions {

class SettingSyncData {
 public:
  
  explicit SettingSyncData(const syncer::SyncChange& sync_change);

  
  explicit SettingSyncData(const syncer::SyncData& sync_data);

  
  SettingSyncData(
      syncer::SyncChange::SyncChangeType change_type,
      const std::string& extension_id,
      const std::string& key,
      scoped_ptr<Value> value);

  ~SettingSyncData();

  
  syncer::SyncChange::SyncChangeType change_type() const;

  
  const std::string& extension_id() const;

  
  const std::string& key() const;

  
  const Value& value() const;

 private:
  
  
  class Internal : public base::RefCountedThreadSafe<Internal> {
   public:
    Internal(
      syncer::SyncChange::SyncChangeType change_type,
      const std::string& extension_id,
      const std::string& key,
      scoped_ptr<Value> value);

    syncer::SyncChange::SyncChangeType change_type_;
    std::string extension_id_;
    std::string key_;
    scoped_ptr<Value> value_;

   private:
    friend class base::RefCountedThreadSafe<Internal>;
    ~Internal();
  };

  
  void Init(syncer::SyncChange::SyncChangeType change_type,
            const syncer::SyncData& sync_data);

  
  void InitFromExtensionSettingSpecifics(
      syncer::SyncChange::SyncChangeType change_type,
      const sync_pb::ExtensionSettingSpecifics& specifics);

  scoped_refptr<Internal> internal_;
};

typedef std::vector<SettingSyncData> SettingSyncDataList;

}  

#endif  
