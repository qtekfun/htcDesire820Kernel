// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_TYPED_URL_CHANGE_PROCESSOR_H_
#define CHROME_BROWSER_SYNC_GLUE_TYPED_URL_CHANGE_PROCESSOR_H_

#include "chrome/browser/sync/glue/change_processor.h"

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/sync/glue/data_type_error_handler.h"
#include "chrome/browser/sync/glue/sync_backend_host.h"
#include "chrome/browser/sync/glue/typed_url_model_associator.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/notification_types.h"

class Profile;

namespace base {
class MessageLoop;
}

namespace content {
class NotificationService;
}

namespace history {
class HistoryBackend;
struct URLsDeletedDetails;
struct URLsModifiedDetails;
struct URLVisitedDetails;
class URLRow;
};

namespace browser_sync {

class DataTypeErrorHandler;

class TypedUrlChangeProcessor : public ChangeProcessor,
                                public content::NotificationObserver {
 public:
  TypedUrlChangeProcessor(Profile* profile,
                          TypedUrlModelAssociator* model_associator,
                          history::HistoryBackend* history_backend,
                          DataTypeErrorHandler* error_handler);
  virtual ~TypedUrlChangeProcessor();

  
  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void ApplyChangesFromSyncModel(
      const syncer::BaseTransaction* trans,
      int64 model_version,
      const syncer::ImmutableChangeRecordList& changes) OVERRIDE;

  
  
  virtual void CommitChangesFromSyncModel() OVERRIDE;

  
  void Disconnect();

 protected:
  virtual void StartImpl(Profile* profile) OVERRIDE;

 private:
  friend class ScopedStopObserving<TypedUrlChangeProcessor>;
  void StartObserving();
  void StopObserving();

  void HandleURLsModified(history::URLsModifiedDetails* details);
  void HandleURLsDeleted(history::URLsDeletedDetails* details);
  void HandleURLsVisited(history::URLVisitedDetails* details);

  
  
  
  
  bool ShouldSyncVisit(history::URLVisitedDetails* details);

  
  
  
  bool CreateOrUpdateSyncNode(history::URLRow typed_url,
                              syncer::WriteTransaction* transaction);

  
  Profile* profile_;

  
  TypedUrlModelAssociator* model_associator_;

  
  
  
  history::HistoryBackend* history_backend_;
  base::MessageLoop* backend_loop_;

  content::NotificationRegistrar notification_registrar_;

  scoped_ptr<content::NotificationService> notification_service_;

  // The set of pending changes that will be written out on the next
  
  history::URLRows pending_new_urls_;
  TypedUrlModelAssociator::TypedUrlUpdateVector pending_updated_urls_;
  std::vector<GURL> pending_deleted_urls_;
  TypedUrlModelAssociator::TypedUrlVisitVector pending_new_visits_;
  history::VisitVector pending_deleted_visits_;

  bool disconnected_;
  base::Lock disconnect_lock_;

  DISALLOW_COPY_AND_ASSIGN(TypedUrlChangeProcessor);
};

}  

#endif  
