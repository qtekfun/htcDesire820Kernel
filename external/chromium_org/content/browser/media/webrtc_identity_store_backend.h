// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_MEDIA_WEBRTC_IDENTITY_STORE_BACKEND_H_
#define CONTENT_BROWSER_MEDIA_WEBRTC_IDENTITY_STORE_BACKEND_H_

#include <map>
#include <string>

#include "base/time/time.h"
#include "sql/connection.h"
#include "sql/meta_table.h"

class GURL;

namespace base {
class FilePath;
}  

namespace quota {
class SpecialStoragePolicy;
}  

namespace content {

class WebRTCIdentityStoreBackend
    : public base::RefCountedThreadSafe<WebRTCIdentityStoreBackend> {
 public:
  typedef base::Callback<void(int error,
                              const std::string& certificate,
                              const std::string& private_key)>
      FindIdentityCallback;

  
  
  WebRTCIdentityStoreBackend(const base::FilePath& path,
                             quota::SpecialStoragePolicy* policy,
                             base::TimeDelta validity_period);

  
  
  
  
  
  
  
  
  bool FindIdentity(const GURL& origin,
                    const std::string& identity_name,
                    const std::string& common_name,
                    const FindIdentityCallback& callback);

  
  
  
  
  
  
  
  
  void AddIdentity(const GURL& origin,
                   const std::string& identity_name,
                   const std::string& common_name,
                   const std::string& certificate,
                   const std::string& private_key);

  
  
  
  void Close();

  
  
  void DeleteBetween(base::Time delete_begin,
                     base::Time delete_end,
                     const base::Closure& callback);

  
  
  void SetValidityPeriodForTesting(base::TimeDelta validity_period);

 private:
  friend class base::RefCountedThreadSafe<WebRTCIdentityStoreBackend>;
  class SqlLiteStorage;
  enum LoadingState {
    NOT_STARTED,
    LOADING,
    LOADED,
    CLOSED,
  };
  struct PendingFindRequest;
  struct IdentityKey;
  struct Identity;
  typedef std::map<IdentityKey, Identity> IdentityMap;

  ~WebRTCIdentityStoreBackend();

  void OnLoaded(scoped_ptr<IdentityMap> out_map);


  
  base::TimeDelta validity_period_;
  
  IdentityMap identities_;
  
  std::vector<PendingFindRequest*> pending_find_requests_;
  
  LoadingState state_;
  
  scoped_refptr<SqlLiteStorage> sql_lite_storage_;

  DISALLOW_COPY_AND_ASSIGN(WebRTCIdentityStoreBackend);
};
}

#endif  
