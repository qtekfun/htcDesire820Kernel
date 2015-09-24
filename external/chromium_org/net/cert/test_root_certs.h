// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_CERT_TEST_ROOT_CERTS_H_
#define NET_CERT_TEST_ROOT_CERTS_H_

#include "base/lazy_instance.h"
#include "base/memory/ref_counted.h"
#include "build/build_config.h"
#include "net/base/net_export.h"

#if defined(USE_NSS) || defined(OS_IOS)
#include <list>
#elif defined(OS_WIN)
#include <windows.h>
#include <wincrypt.h>
#elif defined(OS_MACOSX)
#include <CoreFoundation/CFArray.h>
#include <Security/SecTrust.h>
#include "base/mac/scoped_cftyperef.h"
#endif

namespace base {
class FilePath;
}

namespace net {

class X509Certificate;

class NET_EXPORT_PRIVATE TestRootCerts {
 public:
  
  static TestRootCerts* GetInstance();

  
  static bool HasInstance();

  
  
  bool Add(X509Certificate* certificate);

  
  
  
  bool AddFromFile(const base::FilePath& file);

  
  
  void Clear();

  
  bool IsEmpty() const;

#if defined(OS_MACOSX) && !defined(OS_IOS)
  CFArrayRef temporary_roots() const { return temporary_roots_; }

  
  
  
  OSStatus FixupSecTrustRef(SecTrustRef trust_ref) const;

  
  
  
  void SetAllowSystemTrust(bool allow_system_trust);

#elif defined(OS_WIN)
  HCERTSTORE temporary_roots() const { return temporary_roots_; }

  
  
  
  
  HCERTCHAINENGINE GetChainEngine() const;
#endif

 private:
  friend struct base::DefaultLazyInstanceTraits<TestRootCerts>;

  TestRootCerts();
  ~TestRootCerts();

  
  void Init();

#if defined(USE_NSS) || defined(OS_IOS)
  
  
  class TrustEntry;
  std::list<TrustEntry*> trust_cache_;
#elif defined(OS_WIN)
  HCERTSTORE temporary_roots_;
#elif defined(OS_MACOSX)
  base::ScopedCFTypeRef<CFMutableArrayRef> temporary_roots_;
  bool allow_system_trust_;
#endif

#if defined(OS_WIN) || defined(USE_OPENSSL)
  
  bool empty_;
#endif

  DISALLOW_COPY_AND_ASSIGN(TestRootCerts);
};

class NET_EXPORT_PRIVATE ScopedTestRoot {
 public:
  ScopedTestRoot();
  
  explicit ScopedTestRoot(X509Certificate* cert);
  ~ScopedTestRoot();

  
  
  
  
  
  void Reset(X509Certificate* cert);

 private:
  scoped_refptr<X509Certificate> cert_;

  DISALLOW_COPY_AND_ASSIGN(ScopedTestRoot);
};

}  

#endif  
