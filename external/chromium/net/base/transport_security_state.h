// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_TRANSPORT_SECURITY_STATE_H_
#define NET_BASE_TRANSPORT_SECURITY_STATE_H_
#pragma once

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/time.h"
#include "net/base/x509_cert_types.h"

namespace net {

class TransportSecurityState :
    public base::RefCountedThreadSafe<TransportSecurityState> {
 public:
  TransportSecurityState();

  
  struct DomainState {
    enum Mode {
      
      
      
      MODE_STRICT = 0,
      
      
      
      MODE_OPPORTUNISTIC = 1,
      
      
      
      
      MODE_SPDY_ONLY = 2,
      
      MODE_NONE = 3,
    };

    DomainState();
    ~DomainState();

    
    
    
    
    bool IsChainOfPublicKeysPermitted(
        const std::vector<SHA1Fingerprint>& hashes);

    Mode mode;
    base::Time created;  
    base::Time expiry;  
    bool include_subdomains;  
    std::vector<SHA1Fingerprint> public_key_hashes;  

    
    bool preloaded;  
    std::string domain;  
  };

  
  void EnableHost(const std::string& host, const DomainState& state);

  
  
  bool DeleteHost(const std::string& host);

  
  
  bool IsEnabledForHost(DomainState* result,
                        const std::string& host,
                        bool sni_available);

  
  void DeleteSince(const base::Time& time);

  
  
  
  
  static bool ParseHeader(const std::string& value,
                          int* max_age,
                          bool* include_subdomains);

  class Delegate {
   public:
    
    
    virtual void StateIsDirty(TransportSecurityState* state) = 0;

   protected:
    virtual ~Delegate() {}
  };

  void SetDelegate(Delegate*);

  bool Serialise(std::string* output);
  
  
  bool LoadEntries(const std::string& state, bool* dirty);

  
  static const long int kMaxHSTSAgeSecs;

 private:
  friend class base::RefCountedThreadSafe<TransportSecurityState>;
  FRIEND_TEST_ALL_PREFIXES(TransportSecurityStateTest, IsPreloaded);

  ~TransportSecurityState();

  
  
  void DirtyNotify();

  static std::string CanonicalizeHost(const std::string& host);
  static bool IsPreloadedSTS(const std::string& canonicalized_host,
                             bool sni_available,
                             DomainState* out);
  static bool Deserialise(const std::string& state,
                          bool* dirty,
                          std::map<std::string, DomainState>* out);

  
  
  
  std::map<std::string, DomainState> enabled_hosts_;

  
  Delegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(TransportSecurityState);
};

}  

#endif  
