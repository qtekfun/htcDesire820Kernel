// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_PASSWORD_CHANGE_PROCESSOR_H_
#define CHROME_BROWSER_SYNC_GLUE_PASSWORD_CHANGE_PROCESSOR_H_

#include "chrome/browser/sync/glue/change_processor.h"

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/sync/glue/data_type_error_handler.h"
#include "chrome/browser/sync/glue/password_model_associator.h"
#include "chrome/browser/sync/glue/sync_backend_host.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/notification_types.h"

class PasswordStore;

namespace base {
class MessageLoop;
}

namespace browser_sync {

class DataTypeErrorHandler;

class PasswordChangeProcessor : public ChangeProcessor,
                                public content::NotificationObserver {
 public:
  PasswordChangeProcessor(PasswordModelAssociator* model_associator,
                          PasswordStore* password_store,
                          DataTypeErrorHandler* error_handler);
  virtual ~PasswordChangeProcessor();

  
  
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
  friend class ScopedStopObserving<PasswordChangeProcessor>;
  void StartObserving();
  void StopObserving();

  
  void InitObserving();

  
  PasswordModelAssociator* model_associator_;

  
  
  
  scoped_refptr<PasswordStore> password_store_;

  
  
  PasswordModelAssociator::PasswordVector new_passwords_;
  PasswordModelAssociator::PasswordVector updated_passwords_;
  PasswordModelAssociator::PasswordVector deleted_passwords_;

  content::NotificationRegistrar notification_registrar_;

  base::MessageLoop* expected_loop_;

  
  
  bool disconnected_;
  base::Lock disconnect_lock_;

  DISALLOW_COPY_AND_ASSIGN(PasswordChangeProcessor);
};

}  

#endif  
