// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_WEBDATA_COMMON_WEB_DATA_SERVICE_BASE_H_
#define COMPONENTS_WEBDATA_COMMON_WEB_DATA_SERVICE_BASE_H_

#include "base/callback.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/ref_counted_delete_on_message_loop.h"
#include "base/memory/scoped_ptr.h"
#include "components/webdata/common/webdata_export.h"
#include "sql/init_status.h"

class WebDatabase;
class WebDatabaseService;
class WebDatabaseTable;

namespace base {
class Thread;
}

class WEBDATA_EXPORT WebDataServiceBase
    : public base::RefCountedDeleteOnMessageLoop<WebDataServiceBase> {
 public:
  
  typedef int Handle;

  
  
  
  
  
  typedef base::Callback<void(sql::InitStatus)> ProfileErrorCallback;

  typedef base::Closure DBLoadedCallback;

  
  
  
  
  
  
  
  
  
  WebDataServiceBase(scoped_refptr<WebDatabaseService> wdbs,
                     const ProfileErrorCallback& callback,
                     const scoped_refptr<base::MessageLoopProxy>& ui_thread);

  
  
  virtual void CancelRequest(Handle h);

  
  
  virtual void ShutdownOnUIThread();

  
  virtual void Init();

  
  
  void UnloadDatabase();

  
  void ShutdownDatabase();

  
  
  
  
  
  virtual void RegisterDBLoadedCallback(const DBLoadedCallback& callback);

  
  
  virtual bool IsDatabaseLoaded();

  
  
  
  virtual WebDatabase* GetDatabase();

 protected:
  friend class base::RefCountedDeleteOnMessageLoop<WebDataServiceBase>;
  friend class base::DeleteHelper<WebDataServiceBase>;

  virtual ~WebDataServiceBase();

  
  scoped_refptr<WebDatabaseService> wdbs_;

 private:
  ProfileErrorCallback profile_error_callback_;

  DISALLOW_COPY_AND_ASSIGN(WebDataServiceBase);
};

#endif  
