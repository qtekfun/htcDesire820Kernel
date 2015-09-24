// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_WEBDATA_COMMON_WEB_DATA_SERVICE_BACKEND_H_
#define COMPONENTS_WEBDATA_COMMON_WEB_DATA_SERVICE_BACKEND_H_

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/ref_counted_delete_on_message_loop.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "components/webdata/common/web_database_service.h"
#include "components/webdata/common/webdata_export.h"

class WebDatabase;
class WebDatabaseTable;
class WebDataRequest;
class WebDataRequestManager;

namespace tracked_objects {
class Location;
}



class WEBDATA_EXPORT WebDataServiceBackend
    : public base::RefCountedDeleteOnMessageLoop<WebDataServiceBackend> {
 public:
  class Delegate {
    public:
    virtual ~Delegate() {}

    
    virtual void DBLoaded(sql::InitStatus status) = 0;
  };

  WebDataServiceBackend(const base::FilePath& path,
                        Delegate* delegate,
                        const scoped_refptr<base::MessageLoopProxy>& db_thread);

  
  void AddTable(scoped_ptr<WebDatabaseTable> table);

  
  
  void InitDatabase();

  
  
  
  
  sql::InitStatus LoadDatabaseIfNecessary();

  
  
  void ShutdownDatabase(bool should_reinit);

  
  
  
  
  void DBWriteTaskWrapper(
      const WebDatabaseService::WriteTask& task,
      scoped_ptr<WebDataRequest> request);
  void DBReadTaskWrapper(
      const WebDatabaseService::ReadTask& task,
      scoped_ptr<WebDataRequest> request);

  
  void ExecuteWriteTask(const WebDatabaseService::WriteTask& task);
  scoped_ptr<WDTypedResult> ExecuteReadTask(
      const WebDatabaseService::ReadTask& task);

  const scoped_refptr<WebDataRequestManager>& request_manager() {
    return request_manager_;
  }

  WebDatabase* database() { return db_.get(); }

 protected:
  friend class base::RefCountedDeleteOnMessageLoop<WebDataServiceBackend>;
  friend class base::DeleteHelper<WebDataServiceBackend>;

  virtual ~WebDataServiceBackend();

 private:
  
  void Commit();

  
  base::FilePath db_path_;

  
  
  
  
  
  
  
  
  ScopedVector<WebDatabaseTable> tables_;

  scoped_ptr<WebDatabase> db_;

  
  scoped_refptr<WebDataRequestManager> request_manager_;

  
  
  sql::InitStatus init_status_;

  
  
  bool init_complete_;

  
  scoped_ptr<Delegate> delegate_;

  DISALLOW_COPY_AND_ASSIGN(WebDataServiceBackend);
};

#endif  
