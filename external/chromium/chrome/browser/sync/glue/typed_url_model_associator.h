// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_TYPED_URL_MODEL_ASSOCIATOR_H_
#define CHROME_BROWSER_SYNC_GLUE_TYPED_URL_MODEL_ASSOCIATOR_H_
#pragma once

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/string16.h"
#include "base/task.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/sync/glue/model_associator.h"
#include "chrome/browser/sync/protocol/typed_url_specifics.pb.h"

class GURL;
class MessageLoop;
class ProfileSyncService;

namespace history {
class HistoryBackend;
class URLRow;
};

namespace sync_api {
class WriteNode;
class WriteTransaction;
};

namespace browser_sync {

class TypedUrlChangeProcessor;
class UnrecoverableErrorHandler;

extern const char kTypedUrlTag[];

class TypedUrlModelAssociator
  : public PerDataTypeAssociatorInterface<std::string, std::string> {
 public:
  typedef std::vector<std::pair<GURL, string16> > TypedUrlTitleVector;
  typedef std::vector<history::URLRow> TypedUrlVector;
  typedef std::vector<std::pair<history::URLID, history::URLRow> >
      TypedUrlUpdateVector;
  typedef std::vector<std::pair<GURL, std::vector<base::Time> > >
      TypedUrlVisitVector;

  static syncable::ModelType model_type() { return syncable::TYPED_URLS; }
  TypedUrlModelAssociator(ProfileSyncService* sync_service,
                          history::HistoryBackend* history_backend);
  virtual ~TypedUrlModelAssociator();

  
  
  
  virtual bool AssociateModels();

  
  bool DeleteAllNodes(sync_api::WriteTransaction* trans);

  
  virtual bool DisassociateModels();

  
  
  virtual bool SyncModelHasUserCreatedNodes(bool* has_nodes);

  virtual void AbortAssociation();

  virtual bool CryptoReadyIfNecessary();

  
  virtual const std::string* GetChromeNodeFromSyncId(int64 sync_id);

  
  virtual bool InitSyncNodeFromChromeId(const std::string& node_id,
                                        sync_api::BaseNode* sync_node);

  
  
  virtual int64 GetSyncIdFromChromeId(const std::string& node_id);

  
  virtual void Associate(const std::string* node, int64 sync_id);

  
  virtual void Disassociate(int64 sync_id);

  
  
  virtual bool GetSyncIdForTaggedNode(const std::string& tag, int64* sync_id);

  bool WriteToHistoryBackend(const TypedUrlTitleVector* titles,
                             const TypedUrlVector* new_urls,
                             const TypedUrlUpdateVector* updated_urls,
                             const TypedUrlVisitVector* new_visits,
                             const history::VisitVector* deleted_visits);

  enum {
    DIFF_NONE           = 0x0000,
    DIFF_NODE_CHANGED   = 0x0001,
    DIFF_TITLE_CHANGED  = 0x0002,
    DIFF_ROW_CHANGED    = 0x0004,
    DIFF_VISITS_ADDED   = 0x0008,
  };

  static int MergeUrls(const sync_pb::TypedUrlSpecifics& typed_url,
                       const history::URLRow& url,
                       history::VisitVector* visits,
                       history::URLRow* new_url,
                       std::vector<base::Time>* new_visits);
  static void WriteToSyncNode(const history::URLRow& url,
                              const history::VisitVector& visits,
                              sync_api::WriteNode* node);

  static void DiffVisits(const history::VisitVector& old_visits,
                         const sync_pb::TypedUrlSpecifics& new_url,
                         std::vector<base::Time>* new_visits,
                         history::VisitVector* removed_visits);

 private:
  typedef std::map<std::string, int64> TypedUrlToSyncIdMap;
  typedef std::map<int64, std::string> SyncIdToTypedUrlMap;

  ProfileSyncService* sync_service_;
  history::HistoryBackend* history_backend_;
  int64 typed_url_node_id_;

  MessageLoop* expected_loop_;

  TypedUrlToSyncIdMap id_map_;
  SyncIdToTypedUrlMap id_map_inverse_;

  DISALLOW_COPY_AND_ASSIGN(TypedUrlModelAssociator);
};

}  

#endif  
