// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BROWSING_DATA_BROWSING_DATA_SERVER_BOUND_CERT_HELPER_H_
#define CHROME_BROWSER_BROWSING_DATA_BROWSING_DATA_SERVER_BOUND_CERT_HELPER_H_

#include <map>
#include <string>

#include "base/callback.h"
#include "net/ssl/server_bound_cert_store.h"

class Profile;

class BrowsingDataServerBoundCertHelper
    : public base::RefCountedThreadSafe<BrowsingDataServerBoundCertHelper> {
 public:

  
  
  static BrowsingDataServerBoundCertHelper* Create(Profile* profile);

  typedef base::Callback<
      void(const net::ServerBoundCertStore::ServerBoundCertList&)>
      FetchResultCallback;

  
  
  
  virtual void StartFetching(const FetchResultCallback& callback) = 0;
  
  
  virtual void DeleteServerBoundCert(const std::string& server_id) = 0;

 protected:
  friend class base::RefCountedThreadSafe<BrowsingDataServerBoundCertHelper>;
  virtual ~BrowsingDataServerBoundCertHelper() {}
};

class CannedBrowsingDataServerBoundCertHelper
    : public BrowsingDataServerBoundCertHelper {
 public:
  CannedBrowsingDataServerBoundCertHelper();

  
  
  
  CannedBrowsingDataServerBoundCertHelper* Clone();

  
  
  void AddServerBoundCert(
      const net::ServerBoundCertStore::ServerBoundCert& server_bound_cert);

  
  void Reset();

  
  bool empty() const;

  
  size_t GetCertCount() const;

  
  virtual void StartFetching(const FetchResultCallback& callback) OVERRIDE;
  virtual void DeleteServerBoundCert(const std::string& server_id) OVERRIDE;

 private:
  virtual ~CannedBrowsingDataServerBoundCertHelper();

  void FinishFetching();

  typedef std::map<std::string, net::ServerBoundCertStore::ServerBoundCert>
      ServerBoundCertMap;
  ServerBoundCertMap server_bound_cert_map_;

  FetchResultCallback completion_callback_;

  DISALLOW_COPY_AND_ASSIGN(CannedBrowsingDataServerBoundCertHelper);
};

#endif  
