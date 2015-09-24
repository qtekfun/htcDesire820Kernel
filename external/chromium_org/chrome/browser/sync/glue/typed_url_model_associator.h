// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_TYPED_URL_MODEL_ASSOCIATOR_H_
#define CHROME_BROWSER_SYNC_GLUE_TYPED_URL_MODEL_ASSOCIATOR_H_

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/strings/string16.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/sync/glue/data_type_error_handler.h"
#include "chrome/browser/sync/glue/model_associator.h"
#include "sync/protocol/typed_url_specifics.pb.h"

class GURL;
class ProfileSyncService;

namespace base {
class MessageLoop;
}

namespace history {
class HistoryBackend;
class URLRow;
};

namespace syncer {
class WriteNode;
class WriteTransaction;
};

namespace browser_sync {

extern const char kTypedUrlTag[];

class TypedUrlModelAssociator : public AssociatorInterface {
 public:
  typedef std::vector<std::pair<history::URLID, history::URLRow> >
      TypedUrlUpdateVector;
  typedef std::vector<std::pair<GURL, std::vector<history::VisitInfo> > >
      TypedUrlVisitVector;

  static syncer::ModelType model_type() { return syncer::TYPED_URLS; }
  TypedUrlModelAssociator(ProfileSyncService* sync_service,
                          history::HistoryBackend* history_backend,
                          DataTypeErrorHandler* error_handler);
  virtual ~TypedUrlModelAssociator();

  
  
  
  virtual syncer::SyncError AssociateModels(
      syncer::SyncMergeResult* local_merge_result,
      syncer::SyncMergeResult* syncer_merge_result) OVERRIDE;

  
  virtual syncer::SyncError DisassociateModels() OVERRIDE;

  
  
  virtual void AbortAssociation() OVERRIDE;

  
  
  virtual bool SyncModelHasUserCreatedNodes(bool* has_nodes) OVERRIDE;

  virtual bool CryptoReadyIfNecessary() OVERRIDE;

  
  bool DeleteAllNodes(syncer::WriteTransaction* trans);

  void WriteToHistoryBackend(const history::URLRows* new_urls,
                             const TypedUrlUpdateVector* updated_urls,
                             const TypedUrlVisitVector* new_visits,
                             const history::VisitVector* deleted_visits);

  
  
  
  
  
  
  
  void UpdateFromSyncDB(const sync_pb::TypedUrlSpecifics& typed_url,
                        TypedUrlVisitVector* visits_to_add,
                        history::VisitVector* visits_to_remove,
                        TypedUrlUpdateVector* updated_urls,
                        history::URLRows* new_urls);

  
  
  
  sync_pb::TypedUrlSpecifics FilterExpiredVisits(
      const sync_pb::TypedUrlSpecifics& specifics);

  
  int GetErrorPercentage() const;

  
  typedef uint32 MergeResult;
  static const MergeResult DIFF_NONE                = 0;
  static const MergeResult DIFF_UPDATE_NODE         = 1 << 0;
  static const MergeResult DIFF_LOCAL_ROW_CHANGED   = 1 << 1;
  static const MergeResult DIFF_LOCAL_VISITS_ADDED  = 1 << 2;

  
  
  
  
  
  
  
  
  //   should be written to the history DB for this URL. Deletions are not
  //   written to the DB - each client is left to age out visits on their own.
  static MergeResult MergeUrls(const sync_pb::TypedUrlSpecifics& typed_url,
                               const history::URLRow& url,
                               history::VisitVector* visits,
                               history::URLRow* new_url,
                               std::vector<history::VisitInfo>* new_visits);
  static void WriteToSyncNode(const history::URLRow& url,
                              const history::VisitVector& visits,
                              syncer::WriteNode* node);

  
  
  
  
  
  static void DiffVisits(const history::VisitVector& old_visits,
                         const sync_pb::TypedUrlSpecifics& new_url,
                         std::vector<history::VisitInfo>* new_visits,
                         history::VisitVector* removed_visits);

  
  
  static void WriteToTypedUrlSpecifics(const history::URLRow& url,
                                       const history::VisitVector& visits,
                                       sync_pb::TypedUrlSpecifics* specifics);

  
  
  
  
  
  
  
  bool FixupURLAndGetVisits(history::URLRow* url,
                            history::VisitVector* visits);

  
  
  
  static void UpdateURLRowFromTypedUrlSpecifics(
      const sync_pb::TypedUrlSpecifics& specifics, history::URLRow* url_row);

  
  
  bool ShouldIgnoreUrl(const GURL& url);

 protected:
  
  
  
  virtual void ClearErrorStats();

 private:

  
  syncer::SyncError DoAssociateModels();

  
  
  bool ShouldIgnoreVisits(const history::VisitVector& visits);

  ProfileSyncService* sync_service_;
  history::HistoryBackend* history_backend_;

  base::MessageLoop* expected_loop_;

  bool abort_requested_;
  base::Lock abort_lock_;

  DataTypeErrorHandler* error_handler_; 

  
  
  int num_db_accesses_;
  int num_db_errors_;

  DISALLOW_COPY_AND_ASSIGN(TypedUrlModelAssociator);
};

}  

#endif  
