// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// information and MUST not be extracted, overwritten or modified except

#ifndef COMPONENTS_WEBDATA_COMMON_WEB_DATA_REQUEST_MANAGER_H__
#define COMPONENTS_WEBDATA_COMMON_WEB_DATA_REQUEST_MANAGER_H__

#include <map>

#include "base/memory/ref_counted.h"
#include "base/synchronization/lock.h"
#include "components/webdata/common/web_database_service.h"
#include "components/webdata/common/web_data_results.h"
#include "components/webdata/common/web_data_service_base.h"
#include "components/webdata/common/web_data_service_consumer.h"

class WebDataService;
class WebDataServiceConsumer;
class WebDataRequestManager;

namespace base {
class MessageLoop;
}

class WebDataRequest {
 public:
  WebDataRequest(WebDataServiceConsumer* consumer,
                 WebDataRequestManager* manager);

  virtual ~WebDataRequest();

  WebDataServiceBase::Handle GetHandle() const;

  
  WebDataServiceConsumer* GetConsumer() const;

  
  base::MessageLoop* GetMessageLoop() const;

  
  bool IsCancelled() const;

  
  
  void Cancel();

  
  void OnComplete();

  
  void SetResult(scoped_ptr<WDTypedResult> r);

  
  
  scoped_ptr<WDTypedResult> GetResult();

 private:
  
  
  WebDataRequestManager* manager_;

  
  base::MessageLoop* message_loop_;

  
  WebDataServiceBase::Handle handle_;

  
  
  mutable base::Lock cancel_lock_;
  bool cancelled_;

  
  WebDataServiceConsumer* consumer_;

  scoped_ptr<WDTypedResult> result_;

  DISALLOW_COPY_AND_ASSIGN(WebDataRequest);
};

class WebDataRequestManager
    : public base::RefCountedThreadSafe<WebDataRequestManager> {
 public:
  WebDataRequestManager();

  
  void CancelRequest(WebDataServiceBase::Handle h);

  
  void RequestCompleted(scoped_ptr<WebDataRequest> request);

  
  void RegisterRequest(WebDataRequest* request);

  
  int GetNextRequestHandle();

 private:
  friend class base::RefCountedThreadSafe<WebDataRequestManager>;

  ~WebDataRequestManager();

  
  
  void RequestCompletedOnThread(scoped_ptr<WebDataRequest> request);

  
  base::Lock pending_lock_;

  
  WebDataServiceBase::Handle next_request_handle_;

  typedef std::map<WebDataServiceBase::Handle, WebDataRequest*> RequestMap;
  RequestMap pending_requests_;

  DISALLOW_COPY_AND_ASSIGN(WebDataRequestManager);
};

#endif  
