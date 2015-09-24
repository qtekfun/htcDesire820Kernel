// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_EV_ROOT_CA_METADATA_H_
#define NET_BASE_EV_ROOT_CA_METADATA_H_
#pragma once

#include "build/build_config.h"

#if defined(USE_NSS)
#include <secoidt.h>
#endif

#include <map>
#include <vector>

#include "net/base/x509_certificate.h"

namespace base {
template <typename T>
struct DefaultLazyInstanceTraits;
}  

namespace net {

class EVRootCAMetadata {
 public:
#if defined(USE_NSS)
  typedef SECOidTag PolicyOID;
#else
  typedef const char* PolicyOID;
#endif

  static EVRootCAMetadata* GetInstance();

  
  
  bool GetPolicyOID(const SHA1Fingerprint& fingerprint,
                    PolicyOID* policy_oid) const;

  const PolicyOID* GetPolicyOIDs() const { return &policy_oids_[0]; }
#if defined(OS_WIN)
  int NumPolicyOIDs() const { return num_policy_oids_; }
#else
  int NumPolicyOIDs() const { return policy_oids_.size(); }
#endif

  
  bool IsEVPolicyOID(PolicyOID policy_oid) const;

  
  
  bool HasEVPolicyOID(const SHA1Fingerprint& fingerprint,
                      PolicyOID policy_oid) const;

 private:
  friend struct base::DefaultLazyInstanceTraits<EVRootCAMetadata>;

  typedef std::map<SHA1Fingerprint, PolicyOID,
                   SHA1FingerprintLessThan> PolicyOidMap;

  EVRootCAMetadata();
  ~EVRootCAMetadata();

  static bool PolicyOIDsAreEqual(PolicyOID a, PolicyOID b);

  
  PolicyOidMap ev_policy_;

#if defined(OS_WIN)
  static const PolicyOID policy_oids_[];
  int num_policy_oids_;
#else
  std::vector<PolicyOID> policy_oids_;
#endif

  DISALLOW_COPY_AND_ASSIGN(EVRootCAMetadata);
};

}  

#endif  
