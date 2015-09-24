// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_INTERNAL_API_PUBLIC_DELETE_JOURNAL_H_
#define SYNC_INTERNAL_API_PUBLIC_DELETE_JOURNAL_H_

#include <vector>

#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/model_type.h"
#include "sync/protocol/sync.pb.h"

namespace syncer {

class BaseTransaction;

struct BookmarkDeleteJournal {
  int64 id;   
  bool is_folder;
  sync_pb::EntitySpecifics specifics;
};
typedef std::vector<BookmarkDeleteJournal> BookmarkDeleteJournalList;

class SYNC_EXPORT DeleteJournal {
 public:
  
  
  
  
  
  static void GetBookmarkDeleteJournals(
      BaseTransaction* trans, BookmarkDeleteJournalList *delete_journals);

  
  static void PurgeDeleteJournals(BaseTransaction* trans,
                                  const std::set<int64>& ids);
};

}  

#endif  
