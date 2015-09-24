// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_CERT_EV_ROOT_CA_METADATA_H_
#define NET_CERT_EV_ROOT_CA_METADATA_H_

#include "build/build_config.h"

#if defined(USE_NSS) || defined(OS_IOS)
#include <secoidt.h>
#endif

#include <map>
#include <set>
#include <string>
#include <vector>

#include "net/base/net_export.h"
#include "net/cert/x509_certificate.h"

namespace base {
template <typename T>
struct DefaultLazyInstanceTraits;
}  

namespace net {

class NET_EXPORT_PRIVATE EVRootCAMetadata {
 public:
#if defined(USE_NSS) || defined(OS_IOS)
  typedef SECOidTag PolicyOID;
#elif defined(OS_WIN)
  typedef const char* PolicyOID;
#endif

  static EVRootCAMetadata* GetInstance();

#if defined(USE_NSS) || defined(OS_WIN) || defined(OS_IOS)
  
  bool IsEVPolicyOID(PolicyOID policy_oid) const;

  
  
  bool HasEVPolicyOID(const SHA1HashValue& fingerprint,
                      PolicyOID policy_oid) const;
#endif

  
  
  
  bool AddEVCA(const SHA1HashValue& fingerprint, const char* policy);

  
  
  bool RemoveEVCA(const SHA1HashValue& fingerprint);

 private:
  friend struct base::DefaultLazyInstanceTraits<EVRootCAMetadata>;

  EVRootCAMetadata();
  ~EVRootCAMetadata();

#if defined(USE_NSS) || defined(OS_IOS)
  typedef std::map<SHA1HashValue, std::vector<PolicyOID>,
                   SHA1HashValueLessThan> PolicyOIDMap;

  
  
  static bool RegisterOID(const char* policy, PolicyOID* out);

  PolicyOIDMap ev_policy_;
  std::set<PolicyOID> policy_oids_;
#elif defined(OS_WIN)
  typedef std::map<SHA1HashValue, std::string,
                   SHA1HashValueLessThan> ExtraEVCAMap;

  
  ExtraEVCAMap extra_cas_;
#endif

  DISALLOW_COPY_AND_ASSIGN(EVRootCAMetadata);
};

}  

#endif  
