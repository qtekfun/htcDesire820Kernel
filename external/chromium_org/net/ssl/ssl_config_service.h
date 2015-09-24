// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SSL_SSL_CONFIG_SERVICE_H_
#define NET_SSL_SSL_CONFIG_SERVICE_H_

#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/observer_list.h"
#include "base/strings/string_piece.h"
#include "net/base/net_export.h"
#include "net/cert/cert_status_flags.h"
#include "net/cert/crl_set.h"
#include "net/cert/x509_certificate.h"

namespace net {

enum {
  SSL_PROTOCOL_VERSION_SSL3 = 0x0300,
  SSL_PROTOCOL_VERSION_TLS1 = 0x0301,
  SSL_PROTOCOL_VERSION_TLS1_1 = 0x0302,
  SSL_PROTOCOL_VERSION_TLS1_2 = 0x0303,
};

struct NET_EXPORT SSLConfig {
  
  
  SSLConfig();
  ~SSLConfig();

  
  // The expected cert status is written to |cert_status|. |*cert_status| can
  
  bool IsAllowedBadCert(X509Certificate* cert, CertStatus* cert_status) const;

  
  
  bool IsAllowedBadCert(const base::StringPiece& der_cert,
                        CertStatus* cert_status) const;

  
  
  
  
  
  bool rev_checking_enabled;

  
  
  
  
  
  
  
  bool rev_checking_required_local_anchors;

  
  
  
  
  
  uint16 version_min;
  uint16 version_max;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  std::vector<uint16> disabled_cipher_suites;

  bool cached_info_enabled;  
  bool channel_id_enabled;   
  bool false_start_enabled;  
  
  
  bool signed_cert_timestamps_enabled;

  
  
  
  bool require_forward_secrecy;

  
  
  
  
  
  bool unrestricted_ssl3_fallback_enabled;

  
  

  struct NET_EXPORT CertAndStatus {
    CertAndStatus();
    ~CertAndStatus();

    std::string der_cert;
    CertStatus cert_status;
  };

  
  
  
  
  std::vector<CertAndStatus> allowed_bad_certs;

  
  bool send_client_cert;

  bool verify_ev_cert;  

  bool version_fallback;  
                          
                          

  
  
  
  
  
  bool cert_io_enabled;

  
  
  
  
  
  
  std::vector<std::string> next_protos;

  scoped_refptr<X509Certificate> client_cert;
};

class NET_EXPORT SSLConfigService
    : public base::RefCountedThreadSafe<SSLConfigService> {
 public:
  
  class NET_EXPORT Observer {
   public:
    
    
    
    
    
    
    
    
    
    
    virtual void OnSSLConfigChanged() = 0;

   protected:
    virtual ~Observer() {}
  };

  SSLConfigService();

  
  virtual void GetSSLConfig(SSLConfig* config) = 0;

  
  static void SetCRLSet(scoped_refptr<CRLSet> crl_set);
  static scoped_refptr<CRLSet> GetCRLSet();

  
  
  static void EnableCachedInfo();
  static bool cached_info_enabled();

  
  static uint16 default_version_min();

  
  static uint16 default_version_max();

  
  static bool IsSNIAvailable(SSLConfigService* service);

  
  void AddObserver(Observer* observer);

  
  void RemoveObserver(Observer* observer);

  
  
  void NotifySSLConfigChange();

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
