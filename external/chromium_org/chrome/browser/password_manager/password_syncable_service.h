// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_SYNCABLE_SERVICE_H__
#define CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_SYNCABLE_SERVICE_H__

#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "sync/api/sync_change.h"
#include "sync/api/sync_data.h"
#include "sync/api/sync_error.h"
#include "sync/api/syncable_service.h"
#include "sync/protocol/password_specifics.pb.h"
#include "sync/protocol/sync.pb.h"

namespace autofill {
struct PasswordForm;
}

namespace syncer {
class SyncErrorFactory;
}

class PasswordStore;

class PasswordSyncableService : public syncer::SyncableService {
 public:
  
  
  
  
  
  explicit PasswordSyncableService(
      scoped_refptr<PasswordStore> password_store);
  virtual ~PasswordSyncableService();

  
  virtual syncer::SyncMergeResult MergeDataAndStartSyncing(
      syncer::ModelType type,
      const syncer::SyncDataList& initial_sync_data,
      scoped_ptr<syncer::SyncChangeProcessor> sync_processor,
      scoped_ptr<syncer::SyncErrorFactory> error_handler) OVERRIDE;
  virtual void StopSyncing(syncer::ModelType type) OVERRIDE;
  virtual syncer::SyncDataList GetAllSyncData(
      syncer::ModelType type) const OVERRIDE;
  virtual syncer::SyncError ProcessSyncChanges(
      const tracked_objects::Location& from_here,
      const syncer::SyncChangeList& change_list) OVERRIDE;

  
  
  static std::string MakeTag(const autofill::PasswordForm& password);
  static std::string MakeTag(const sync_pb::PasswordSpecificsData& password);
  static std::string MakeTag(const std::string& origin_url,
                             const std::string& username_element,
                             const std::string& username_value,
                             const std::string& password_element,
                             const std::string& signon_realm);

 private:
  typedef std::vector<autofill::PasswordForm*> PasswordForms;

  
  void WriteToPasswordStore(PasswordForms* new_entries,
                            PasswordForms* udpated_entries);

  
  syncer::SyncData CreateSyncData(const autofill::PasswordForm& password);

  
  
  scoped_ptr<syncer::SyncErrorFactory> sync_error_factory_;

  
  scoped_ptr<syncer::SyncChangeProcessor> sync_processor_;

  
  scoped_refptr<PasswordStore> password_store_;
};

#endif  

