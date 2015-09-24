// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_CERT_VERIFIER_H_
#define NET_BASE_CERT_VERIFIER_H_
#pragma once

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "base/time.h"
#include "net/base/cert_database.h"
#include "net/base/cert_verify_result.h"
#include "net/base/completion_callback.h"
#include "net/base/net_export.h"
#include "net/base/x509_cert_types.h"

namespace net {

class CertVerifierJob;
class CertVerifierWorker;
class X509Certificate;

struct CachedCertVerifyResult {
  CachedCertVerifyResult();
  ~CachedCertVerifyResult();

  
  bool HasExpired(base::Time current_time) const;

  int error;  
  CertVerifyResult result;  

  
  base::Time expiry;
};

class NET_EXPORT CertVerifier : public base::NonThreadSafe,
                     public CertDatabase::Observer {
 public:
  
  typedef void* RequestHandle;

  
  
  class TimeService {
   public:
    virtual ~TimeService() {}

    virtual base::Time Now() = 0;
  };

  CertVerifier();

  
  
  explicit CertVerifier(TimeService* time_service);

  
  
  ~CertVerifier();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int Verify(X509Certificate* cert,
             const std::string& hostname,
             int flags,
             CertVerifyResult* verify_result,
             CompletionCallback* callback,
             RequestHandle* out_req);

  
  
  void CancelRequest(RequestHandle req);

  
  void ClearCache();

  size_t GetCacheSize() const;

  uint64 requests() const { return requests_; }
  uint64 cache_hits() const { return cache_hits_; }
  uint64 inflight_joins() const { return inflight_joins_; }

 private:
  friend class CertVerifierWorker;  

  
  struct RequestParams {
    bool operator==(const RequestParams& other) const {
      
      
      
      return (flags == other.flags &&
              memcmp(cert_fingerprint.data, other.cert_fingerprint.data,
                     sizeof(cert_fingerprint.data)) == 0 &&
              hostname == other.hostname);
    }

    bool operator<(const RequestParams& other) const {
      
      
      
      if (flags != other.flags)
        return flags < other.flags;
      int rv = memcmp(cert_fingerprint.data, other.cert_fingerprint.data,
                      sizeof(cert_fingerprint.data));
      if (rv != 0)
        return rv < 0;
      return hostname < other.hostname;
    }

    SHA1Fingerprint cert_fingerprint;
    std::string hostname;
    int flags;
  };

  void HandleResult(X509Certificate* cert,
                    const std::string& hostname,
                    int flags,
                    int error,
                    const CertVerifyResult& verify_result);

  
  virtual void OnCertTrustChanged(const X509Certificate* cert);

  
  
  std::map<RequestParams, CachedCertVerifyResult> cache_;

  
  
  std::map<RequestParams, CertVerifierJob*> inflight_;

  scoped_ptr<TimeService> time_service_;

  uint64 requests_;
  uint64 cache_hits_;
  uint64 inflight_joins_;

  DISALLOW_COPY_AND_ASSIGN(CertVerifier);
};

class SingleRequestCertVerifier {
 public:
  
  explicit SingleRequestCertVerifier(CertVerifier* cert_verifier);

  
  
  
  ~SingleRequestCertVerifier();

  
  
  int Verify(X509Certificate* cert,
             const std::string& hostname,
             int flags,
             CertVerifyResult* verify_result,
             CompletionCallback* callback);

 private:
  
  
  void OnVerifyCompletion(int result);

  
  CertVerifier* const cert_verifier_;

  
  CertVerifier::RequestHandle cur_request_;
  CompletionCallback* cur_request_callback_;

  
  CompletionCallbackImpl<SingleRequestCertVerifier> callback_;

  DISALLOW_COPY_AND_ASSIGN(SingleRequestCertVerifier);
};

}  

#endif  
