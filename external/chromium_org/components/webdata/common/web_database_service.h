// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// information and MUST not be extracted, overwritten or modified except

#ifndef COMPONENTS_WEBDATA_COMMON_WEB_DATABASE_SERVICE_H_
#define COMPONENTS_WEBDATA_COMMON_WEB_DATABASE_SERVICE_H_

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/ref_counted_delete_on_message_loop.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/message_loop/message_loop_proxy.h"
#include "base/observer_list.h"
#include "components/webdata/common/web_data_service_base.h"
#include "components/webdata/common/web_database.h"
#include "components/webdata/common/webdata_export.h"

class WebDataServiceBackend;
class WebDataRequestManager;

namespace content {
class BrowserContext;
}

namespace tracked_objects {
class Location;
}

class WDTypedResult;
class WebDataServiceConsumer;



class WEBDATA_EXPORT WebDatabaseService
    : public base::RefCountedDeleteOnMessageLoop<WebDatabaseService> {
 public:
  typedef base::Callback<scoped_ptr<WDTypedResult>(WebDatabase*)> ReadTask;
  typedef base::Callback<WebDatabase::State(WebDatabase*)> WriteTask;

  
  typedef base::Closure DBLoadedCallback;
  typedef base::Callback<void(sql::InitStatus)> DBLoadErrorCallback;

  
  
  WebDatabaseService(const base::FilePath& path,
                     const scoped_refptr<base::MessageLoopProxy>& ui_thread,
                     const scoped_refptr<base::MessageLoopProxy>& db_thread);

  
  
  
  virtual void AddTable(scoped_ptr<WebDatabaseTable> table);

  
  virtual void LoadDatabase();

  
  
  virtual void UnloadDatabase();

  
  virtual void ShutdownDatabase();

  
  
  virtual WebDatabase* GetDatabaseOnDB() const;

  
  scoped_refptr<WebDataServiceBackend> GetBackend() const;

  
  virtual void ScheduleDBTask(
      const tracked_objects::Location& from_here,
      const WriteTask& task);

  
  virtual WebDataServiceBase::Handle ScheduleDBTaskWithResult(
      const tracked_objects::Location& from_here,
      const ReadTask& task,
      WebDataServiceConsumer* consumer);

  
  
  
  virtual void CancelRequest(WebDataServiceBase::Handle h);

  
  
  
  
  
  void RegisterDBLoadedCallback(const DBLoadedCallback& callback);

  
  
  
  
  
  void RegisterDBErrorCallback(const DBLoadErrorCallback& callback);

  bool db_loaded() const { return db_loaded_; };

 private:
  class BackendDelegate;
  friend class BackendDelegate;
  friend class base::RefCountedDeleteOnMessageLoop<WebDatabaseService>;
  friend class base::DeleteHelper<WebDatabaseService>;

  typedef std::vector<DBLoadedCallback> LoadedCallbacks;
  typedef std::vector<DBLoadErrorCallback> ErrorCallbacks;

  virtual ~WebDatabaseService();

  void OnDatabaseLoadDone(sql::InitStatus status);

  base::FilePath path_;

  
  
  scoped_refptr<WebDataServiceBackend> wds_backend_;

  
  base::WeakPtrFactory<WebDatabaseService> weak_ptr_factory_;

  
  LoadedCallbacks loaded_callbacks_;

  
  ErrorCallbacks error_callbacks_;

  
  bool db_loaded_;

  scoped_refptr<base::MessageLoopProxy> db_thread_;

  DISALLOW_COPY_AND_ASSIGN(WebDatabaseService);
};

#endif  
