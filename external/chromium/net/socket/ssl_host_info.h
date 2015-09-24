// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_SSL_HOST_INFO_H_
#define NET_SOCKET_SSL_HOST_INFO_H_

#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/time.h"
#include "net/base/cert_verifier.h"
#include "net/base/cert_verify_result.h"
#include "net/base/completion_callback.h"
#include "net/base/dnsrr_resolver.h"
#include "net/socket/ssl_client_socket.h"

namespace net {

class X509Certificate;
struct SSLConfig;

class SSLHostInfo {
 public:
  SSLHostInfo(const std::string& hostname,
              const SSLConfig& ssl_config,
              CertVerifier *certVerifier);
  virtual ~SSLHostInfo();

  
  
  
  virtual void Start() = 0;

  
  
  
  
  
  
  
  
  
  
  virtual int WaitForDataReady(CompletionCallback* callback) = 0;

  
  
  
  
  
  virtual void Persist() = 0;

  
  void StartDnsLookup(DnsRRResolver* dnsrr_resolver);

  struct State {
    State();
    ~State();

    void Clear();

    
    
    std::vector<std::string> certs;

   private:
    DISALLOW_COPY_AND_ASSIGN(State);
  };

  
  
  const State& state() const;
  State* mutable_state();

  
  
  const CertVerifyResult& cert_verify_result() const;

  
  
  
  
  
  int WaitForCertVerification(CompletionCallback* callback);

  base::TimeTicks verification_start_time() const {
    return verification_start_time_;
  }

  base::TimeTicks verification_end_time() const {
    return verification_end_time_;
  }

 protected:
  
  
  
  bool Parse(const std::string& data);
  std::string Serialize() const;
  State state_;
  bool cert_verification_complete_;
  int cert_verification_error_;

 private:
  
  void VerifyCallback(int rv);

  
  bool ParseInner(const std::string& data);

  
  const std::string hostname_;
  bool cert_parsing_failed_;
  CompletionCallback* cert_verification_callback_;
  
  bool rev_checking_enabled_;
  bool verify_ev_cert_;
  base::TimeTicks verification_start_time_;
  base::TimeTicks verification_end_time_;
  CertVerifyResult cert_verify_result_;
  SingleRequestCertVerifier verifier_;
  scoped_refptr<X509Certificate> cert_;
  scoped_refptr<CancelableCompletionCallback<SSLHostInfo> > callback_;

  DnsRRResolver* dnsrr_resolver_;
  CompletionCallback* dns_callback_;
  DnsRRResolver::Handle dns_handle_;
  RRResponse dns_response_;
  base::TimeTicks dns_lookup_start_time_;
  base::TimeTicks cert_verification_finished_time_;
};

class SSLHostInfoFactory {
 public:
  virtual ~SSLHostInfoFactory();

  
  
  virtual SSLHostInfo* GetForHost(const std::string& hostname,
                                  const SSLConfig& ssl_config) = 0;
};

}  

#endif  
