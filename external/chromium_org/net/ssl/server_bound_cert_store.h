// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SSL_SERVER_BOUND_CERT_STORE_H_
#define NET_SSL_SERVER_BOUND_CERT_STORE_H_

#include <list>
#include <string>

#include "base/callback.h"
#include "base/threading/non_thread_safe.h"
#include "base/time/time.h"
#include "net/base/net_export.h"

namespace net {


class NET_EXPORT ServerBoundCertStore
    : NON_EXPORTED_BASE(public base::NonThreadSafe) {
 public:
  
  
  class NET_EXPORT ServerBoundCert {
   public:
    ServerBoundCert();
    ServerBoundCert(const std::string& server_identifier,
                    base::Time creation_time,
                    base::Time expiration_time,
                    const std::string& private_key,
                    const std::string& cert);
    ~ServerBoundCert();

    
    const std::string& server_identifier() const { return server_identifier_; }
    
    
    base::Time creation_time() const { return creation_time_; }
    
    base::Time expiration_time() const { return expiration_time_; }
    
    
    
    const std::string& private_key() const { return private_key_; }
    
    const std::string& cert() const { return cert_; }

   private:
    std::string server_identifier_;
    base::Time creation_time_;
    base::Time expiration_time_;
    std::string private_key_;
    std::string cert_;
  };

  typedef std::list<ServerBoundCert> ServerBoundCertList;

  typedef base::Callback<void(
      int,
      const std::string&,
      base::Time,
      const std::string&,
      const std::string&)> GetCertCallback;
  typedef base::Callback<void(const ServerBoundCertList&)> GetCertListCallback;

  virtual ~ServerBoundCertStore() {}

  
  
  
  
  
  
  virtual int GetServerBoundCert(
      const std::string& server_identifier,
      base::Time* expiration_time,
      std::string* private_key_result,
      std::string* cert_result,
      const GetCertCallback& callback) = 0;

  
  virtual void SetServerBoundCert(
      const std::string& server_identifier,
      base::Time creation_time,
      base::Time expiration_time,
      const std::string& private_key,
      const std::string& cert) = 0;

  
  
  virtual void DeleteServerBoundCert(
      const std::string& server_identifier,
      const base::Closure& completion_callback) = 0;

  
  
  
  virtual void DeleteAllCreatedBetween(
      base::Time delete_begin,
      base::Time delete_end,
      const base::Closure& completion_callback) = 0;

  
  
  virtual void DeleteAll(const base::Closure& completion_callback) = 0;

  
  virtual void GetAllServerBoundCerts(const GetCertListCallback& callback) = 0;

  
  void InitializeFrom(const ServerBoundCertList& list);

  
  
  
  virtual int GetCertCount() = 0;

  
  
  virtual void SetForceKeepSessionState() = 0;
};

}  

#endif  
