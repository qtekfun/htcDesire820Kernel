// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_TRANSPORT_SECURITY_STATE_H_
#define NET_HTTP_TRANSPORT_SECURITY_STATE_H_

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/threading/non_thread_safe.h"
#include "base/time/time.h"
#include "net/base/net_export.h"
#include "net/cert/x509_cert_types.h"
#include "net/cert/x509_certificate.h"

namespace net {

class SSLInfo;

class NET_EXPORT TransportSecurityState
    : NON_EXPORTED_BASE(public base::NonThreadSafe) {
 public:
  class NET_EXPORT Delegate {
   public:
    
    
    virtual void StateIsDirty(TransportSecurityState* state) = 0;

   protected:
    virtual ~Delegate() {}
  };

  TransportSecurityState();
  ~TransportSecurityState();

  
  
  class NET_EXPORT DomainState {
   public:
    enum UpgradeMode {
      
      MODE_FORCE_HTTPS = 0,
      MODE_DEFAULT = 1,
    };

    DomainState();
    ~DomainState();

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    bool CheckPublicKeyPins(const HashValueVector& hashes) const;

    
    
    
    bool HasPublicKeyPins() const;

    
    
    
    bool ShouldUpgradeToSSL() const;

    
    
    bool ShouldSSLErrorsBeFatal() const;

    UpgradeMode upgrade_mode;

    
    
    
    base::Time created;

    
    
    base::Time upgrade_expiry;

    
    
    bool sts_include_subdomains;

    
    
    bool pkp_include_subdomains;

    
    
    
    
    
    
    
    
    HashValueVector static_spki_hashes;

    
    HashValueVector dynamic_spki_hashes;

    
    base::Time dynamic_spki_hashes_expiry;

    
    
    
    HashValueVector bad_static_spki_hashes;

    

    
    
    
    std::string domain;
  };

  class NET_EXPORT Iterator {
   public:
    explicit Iterator(const TransportSecurityState& state);
    ~Iterator();

    bool HasNext() const { return iterator_ != end_; }
    void Advance() { ++iterator_; }
    const std::string& hostname() const { return iterator_->first; }
    const DomainState& domain_state() const { return iterator_->second; }

   private:
    std::map<std::string, DomainState>::const_iterator iterator_;
    std::map<std::string, DomainState>::const_iterator end_;
  };

  
  
  
  
  
  void SetDelegate(Delegate* delegate);

  
  
  
  
  
  
  void ClearDynamicData();

  
  
  
  
  
  void AddOrUpdateEnabledHosts(const std::string& hashed_host,
                               const DomainState& state);

  
  
  
  
  
  void DeleteAllDynamicDataSince(const base::Time& time);

  
  
  
  
  
  
  
  bool DeleteDynamicDataForHost(const std::string& host);

  
  
  
  
  
  
  
  
  
  
  
  bool GetDomainState(const std::string& host,
                      bool sni_enabled,
                      DomainState* result);

  
  
  bool AddHSTSHeader(const std::string& host, const std::string& value);

  
  
  
  bool AddHPKPHeader(const std::string& host, const std::string& value,
                     const SSLInfo& ssl_info);

  
  
  bool AddHSTS(const std::string& host, const base::Time& expiry,
               bool include_subdomains);

  
  
  bool AddHPKP(const std::string& host, const base::Time& expiry,
               bool include_subdomains, const HashValueVector& hashes);

  
  
  
  
  
  
  
  
  
  static bool IsGooglePinnedProperty(const std::string& host,
                                     bool sni_enabled);

  
  static const long int kMaxHSTSAgeSecs;

  
  
  
  
  
  
  
  
  static void ReportUMAOnPinFailure(const std::string& host);

  
  
  
  static bool IsBuildTimely();

 private:
  friend class TransportSecurityStateTest;
  FRIEND_TEST_ALL_PREFIXES(HttpSecurityHeadersTest,
                           UpdateDynamicPKPOnly);

  typedef std::map<std::string, DomainState> DomainStateMap;

  
  
  void DirtyNotify();

  
  
  
  
  void EnableHost(const std::string& host, const DomainState& state);

  
  
  
  static std::string CanonicalizeHost(const std::string& hostname);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool GetStaticDomainState(const std::string& host,
                            bool sni_enabled,
                            DomainState* result);

  
  
  
  
  
  
  
  
  
  
  
  
  bool GetDynamicDomainState(const std::string& host, DomainState* result);

  
  DomainStateMap enabled_hosts_;

  Delegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(TransportSecurityState);
};

}  

#endif  
