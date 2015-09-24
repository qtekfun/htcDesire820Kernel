// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_MEDIA_WEBRTC_IDENTITY_STORE_H_
#define CONTENT_BROWSER_MEDIA_WEBRTC_IDENTITY_STORE_H_

#include <string>
#include <vector>

#include "base/callback.h"
#include "base/time/time.h"
#include "content/common/content_export.h"

class GURL;

namespace base {
class FilePath;
class TaskRunner;
}  

namespace quota {
class SpecialStoragePolicy;
}  

namespace content {
class WebRTCIdentityRequest;
struct WebRTCIdentityRequestResult;
class WebRTCIdentityStoreBackend;
class WebRTCIdentityStoreTest;

class CONTENT_EXPORT WebRTCIdentityStore
    : public base::RefCountedThreadSafe<WebRTCIdentityStore> {
 public:
  typedef base::Callback<void(int error,
                              const std::string& certificate,
                              const std::string& private_key)>
      CompletionCallback;

  
  WebRTCIdentityStore(const base::FilePath& path,
                      quota::SpecialStoragePolicy* policy);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual base::Closure RequestIdentity(const GURL& origin,
                                        const std::string& identity_name,
                                        const std::string& common_name,
                                        const CompletionCallback& callback);

  
  
  void DeleteBetween(base::Time delete_begin,
                     base::Time delete_end,
                     const base::Closure& callback);

 protected:
  
  virtual ~WebRTCIdentityStore();

 private:
  friend class base::RefCountedThreadSafe<WebRTCIdentityStore>;
  friend class WebRTCIdentityStoreTest;

  void SetValidityPeriodForTesting(base::TimeDelta validity_period);
  void SetTaskRunnerForTesting(
      const scoped_refptr<base::TaskRunner>& task_runner);

  void BackendFindCallback(WebRTCIdentityRequest* request,
                           int error,
                           const std::string& certificate,
                           const std::string& private_key);
  void GenerateIdentityCallback(WebRTCIdentityRequest* request,
                                WebRTCIdentityRequestResult* result);
  WebRTCIdentityRequest* FindRequest(const GURL& origin,
                                     const std::string& identity_name,
                                     const std::string& common_name);
  void PostRequestResult(WebRTCIdentityRequest* request,
                         const WebRTCIdentityRequestResult& result);

  
  base::TimeDelta validity_period_;

  
  scoped_refptr<base::TaskRunner> task_runner_;

  
  
  std::vector<WebRTCIdentityRequest*> in_flight_requests_;

  scoped_refptr<WebRTCIdentityStoreBackend> backend_;

  DISALLOW_COPY_AND_ASSIGN(WebRTCIdentityStore);
};

}  

#endif  
