// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_ANDROID_ANDROID_HISTORY_PROVIDER_SERVICE_H_
#define CHROME_BROWSER_HISTORY_ANDROID_ANDROID_HISTORY_PROVIDER_SERVICE_H_

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/common/cancelable_request.h"
#include "chrome/browser/history/android/android_history_types.h"
#include "sql/statement.h"

class Profile;

class AndroidHistoryProviderService : public CancelableRequestProvider {
 public:
  explicit AndroidHistoryProviderService(Profile* profile);
  virtual ~AndroidHistoryProviderService();

  
  typedef base::Callback<void(
                    Handle,                
                    bool,                  
                    history::AndroidStatement*)>  
                    QueryCallback;
  typedef CancelableRequest<QueryCallback> QueryRequest;

  typedef base::Callback<void(
                    Handle,                
                    bool,                  
                    int)>                  
                    UpdateCallback;
  typedef CancelableRequest<UpdateCallback> UpdateRequest;

  typedef base::Callback<void(
                    Handle,                
                    bool,                  
                    int64)>                
                    InsertCallback;
  typedef CancelableRequest<InsertCallback> InsertRequest;

  typedef base::Callback<void(
                    Handle,                
                    bool,                  
                    int)>                  
                    DeleteCallback;
  typedef CancelableRequest<DeleteCallback> DeleteRequest;

  typedef base::Callback<void(
                    Handle,                
                    int)>                  
                    MoveStatementCallback;
  typedef CancelableRequest<MoveStatementCallback> MoveStatementRequest;

  
  
  
  
  
  
  
  
  
  Handle QueryHistoryAndBookmarks(
      const std::vector<history::HistoryAndBookmarkRow::ColumnID>& projections,
      const std::string& selection,
      const std::vector<base::string16>& selection_args,
      const std::string& sort_order,
      CancelableRequestConsumerBase* consumer,
      const QueryCallback& callback);

  
  
  
  
  
  
  Handle UpdateHistoryAndBookmarks(
      const history::HistoryAndBookmarkRow& row,
      const std::string& selection,
      const std::vector<base::string16>& selection_args,
      CancelableRequestConsumerBase* consumer,
      const UpdateCallback& callback);

  
  
  
  
  
  
  
  Handle DeleteHistoryAndBookmarks(
      const std::string& selection,
      const std::vector<base::string16>& selection_args,
      CancelableRequestConsumerBase* consumer,
      const DeleteCallback& callback);

  
  
  Handle InsertHistoryAndBookmark(const history::HistoryAndBookmarkRow& values,
                                  CancelableRequestConsumerBase* consumer,
                                  const InsertCallback& callback);

  
  
  Handle DeleteHistory(const std::string& selection,
                       const std::vector<base::string16>& selection_args,
                       CancelableRequestConsumerBase* consumer,
                       const DeleteCallback& callback);

  
  
  
  
  Handle MoveStatement(history::AndroidStatement* statement,
                       int current_pos,
                       int destination,
                       CancelableRequestConsumerBase* consumer,
                       const MoveStatementCallback& callback);

  
  
  void CloseStatement(history::AndroidStatement* statement);

  
  
  
  Handle InsertSearchTerm(const history::SearchRow& row,
                          CancelableRequestConsumerBase* consumer,
                          const InsertCallback& callback);

  
  
  
  
  
  
  Handle UpdateSearchTerms(const history::SearchRow& row,
                           const std::string& selection,
                           const std::vector<base::string16>& selection_args,
                           CancelableRequestConsumerBase* consumer,
                           const UpdateCallback& callback);

  
  
  
  
  
  
  Handle DeleteSearchTerms(const std::string& selection,
                           const std::vector<base::string16>& selection_args,
                           CancelableRequestConsumerBase* consumer,
                           const DeleteCallback& callback);

  
  
  
  
  
  
  Handle QuerySearchTerms(
      const std::vector<history::SearchRow::ColumnID>& projections,
      const std::string& selection,
      const std::vector<base::string16>& selection_args,
      const std::string& sort_order,
      CancelableRequestConsumerBase* consumer,
      const QueryCallback& callback);

 private:
  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(AndroidHistoryProviderService);
};

#endif  
