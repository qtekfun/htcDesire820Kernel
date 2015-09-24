// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_TEST_ROOT_CERTS_H_
#define NET_BASE_TEST_ROOT_CERTS_H_
#pragma once

#include "base/lazy_instance.h"
#include "build/build_config.h"

#if defined(OS_WIN)
#include <windows.h>
#include <wincrypt.h>
#elif defined(OS_MACOSX)
#include <CoreFoundation/CFArray.h>
#include <Security/SecTrust.h>
#include "base/mac/scoped_cftyperef.h"
#elif defined(USE_NSS)
#include <list>
#endif

class FilePath;

namespace net {

class X509Certificate;

class TestRootCerts {
 public:
  
  static TestRootCerts* GetInstance();

  
  static bool HasInstance();

  
  
  bool Add(X509Certificate* certificate);

  
  
  
  bool AddFromFile(const FilePath& file);

  
  
  void Clear();

  
  bool IsEmpty() const;

#if defined(OS_MACOSX)
  CFArrayRef temporary_roots() const { return temporary_roots_; }

  
  
  
  OSStatus FixupSecTrustRef(SecTrustRef trust_ref) const;
#elif defined(OS_WIN)
  HCERTSTORE temporary_roots() const { return temporary_roots_; }

  
  
  
  
  HCERTCHAINENGINE GetChainEngine() const;
#endif

 private:
  friend struct base::DefaultLazyInstanceTraits<TestRootCerts>;

  TestRootCerts();
  ~TestRootCerts();

  
  void Init();

#if defined(OS_MACOSX)
  base::mac::ScopedCFTypeRef<CFMutableArrayRef> temporary_roots_;
#elif defined(OS_WIN)
  HCERTSTORE temporary_roots_;
#elif defined(USE_NSS)
  
  
  class TrustEntry;
  std::list<TrustEntry*> trust_cache_;
#endif

#if defined(OS_WIN) || defined(USE_OPENSSL)
  
  bool empty_;
#endif

  DISALLOW_COPY_AND_ASSIGN(TestRootCerts);
};

}  

#endif  
