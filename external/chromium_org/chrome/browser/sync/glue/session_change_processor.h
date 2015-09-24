// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_SESSION_CHANGE_PROCESSOR_H_
#define CHROME_BROWSER_SYNC_GLUE_SESSION_CHANGE_PROCESSOR_H_

#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/sync/glue/change_processor.h"
#include "chrome/browser/sync/glue/data_type_error_handler.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/notification_types.h"

class Profile;

namespace content {
class WebContents;
}

namespace browser_sync {

class DataTypeErrorHandler;
class SessionModelAssociator;
class SyncedTabDelegate;

class SessionChangeProcessor : public ChangeProcessor,
                               public content::NotificationObserver {
 public:
  
  SessionChangeProcessor(
      DataTypeErrorHandler* error_handler,
      SessionModelAssociator* session_model_associator);
  
  SessionChangeProcessor(
      DataTypeErrorHandler* error_handler,
      SessionModelAssociator* session_model_associator,
      bool setup_for_test);
  virtual ~SessionChangeProcessor();

  
  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  virtual void ApplyChangesFromSyncModel(
      const syncer::BaseTransaction* trans,
      int64 model_version,
      const syncer::ImmutableChangeRecordList& changes) OVERRIDE;

 protected:
  
  virtual void StartImpl(Profile* profile) OVERRIDE;

 private:
  void OnNavigationBlocked(content::WebContents* web_contents);

  
  void ProcessModifiedTabs(
      const std::vector<SyncedTabDelegate*>& modified_tabs);

  void StartObserving();

  SessionModelAssociator* session_model_associator_;
  content::NotificationRegistrar notification_registrar_;

  
  Profile* profile_;

  
  bool setup_for_test_;

  base::WeakPtrFactory<SessionChangeProcessor> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(SessionChangeProcessor);
};

}  

#endif  
