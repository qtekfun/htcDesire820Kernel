// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_SSL_CONFIG_SERVICE_H_
#define NET_BASE_SSL_CONFIG_SERVICE_H_
#pragma once

#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/observer_list.h"
#include "net/base/net_export.h"
#include "net/base/x509_certificate.h"

namespace net {

struct NET_EXPORT SSLConfig {
  
  
  SSLConfig();
  ~SSLConfig();

  
  bool IsAllowedBadCert(X509Certificate* cert) const;

  bool rev_checking_enabled;  
                              
  
  bool ssl3_enabled;  
  bool tls1_enabled;  
  bool dnssec_enabled;  
  
  bool dns_cert_provenance_checking_enabled;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  std::vector<uint16> disabled_cipher_suites;

  bool false_start_enabled;  

  
  

  struct CertAndStatus {
    CertAndStatus();
    ~CertAndStatus();

    scoped_refptr<X509Certificate> cert;
    int cert_status;
  };

  
  
  
  
  std::vector<CertAndStatus> allowed_bad_certs;

  
  bool send_client_cert;

  bool verify_ev_cert;  

  bool ssl3_fallback;  
                       

  
  
  
  
  
  
  std::string next_protos;

  scoped_refptr<X509Certificate> client_cert;
};

class NET_EXPORT SSLConfigService : public base::RefCountedThreadSafe<SSLConfigService> {
 public:
  
  class NET_EXPORT Observer {
   public:
    
    
    
    
    
    
    virtual void OnSSLConfigChanged() = 0;

   protected:
    virtual ~Observer() {}
  };

  SSLConfigService();

  
  
  
  
  
  static SSLConfigService* CreateSystemSSLConfigService();

  
  virtual void GetSSLConfig(SSLConfig* config) = 0;

  
  
  static bool IsKnownFalseStartIncompatibleServer(const std::string& hostname);

  
  
  static void EnableDNSSEC();
  static bool dnssec_enabled();

  
  static void DisableFalseStart();
  
  static bool false_start_enabled();

  
  static void EnableDNSCertProvenanceChecking();
  static bool dns_cert_provenance_checking_enabled();

  
  void AddObserver(Observer* observer);

  
  void RemoveObserver(Observer* observer);

 protected:
  friend class base::RefCountedThreadSafe<SSLConfigService>;

  virtual ~SSLConfigService();

  
  static void SetSSLConfigFlags(SSLConfig* ssl_config);

  
  void ProcessConfigUpdate(const SSLConfig& orig_config,
                           const SSLConfig& new_config);

 private:
  ObserverList<Observer> observer_list_;
};

}  

#endif  
