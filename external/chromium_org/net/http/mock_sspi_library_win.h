// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_MOCK_SSPI_LIBRARY_WIN_H_
#define NET_HTTP_MOCK_SSPI_LIBRARY_WIN_H_

#include <list>
#include <set>

#include "net/http/http_auth_sspi_win.h"

namespace net {

class MockSSPILibrary : public SSPILibrary {
 public:
  MockSSPILibrary();
  virtual ~MockSSPILibrary();

  
  
  
  virtual SECURITY_STATUS AcquireCredentialsHandle(LPWSTR pszPrincipal,
                                                   LPWSTR pszPackage,
                                                   unsigned long fCredentialUse,
                                                   void* pvLogonId,
                                                   void* pvAuthData,
                                                   SEC_GET_KEY_FN pGetKeyFn,
                                                   void* pvGetKeyArgument,
                                                   PCredHandle phCredential,
                                                   PTimeStamp ptsExpiry);
  virtual SECURITY_STATUS InitializeSecurityContext(PCredHandle phCredential,
                                                    PCtxtHandle phContext,
                                                    SEC_WCHAR* pszTargetName,
                                                    unsigned long fContextReq,
                                                    unsigned long Reserved1,
                                                    unsigned long TargetDataRep,
                                                    PSecBufferDesc pInput,
                                                    unsigned long Reserved2,
                                                    PCtxtHandle phNewContext,
                                                    PSecBufferDesc pOutput,
                                                    unsigned long* contextAttr,
                                                    PTimeStamp ptsExpiry);
  virtual SECURITY_STATUS QuerySecurityPackageInfo(LPWSTR pszPackageName,
                                                   PSecPkgInfoW *pkgInfo);
  virtual SECURITY_STATUS FreeCredentialsHandle(PCredHandle phCredential);
  virtual SECURITY_STATUS DeleteSecurityContext(PCtxtHandle phContext);
  virtual SECURITY_STATUS FreeContextBuffer(PVOID pvContextBuffer);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void ExpectQuerySecurityPackageInfo(const std::wstring& expected_package,
                                      SECURITY_STATUS response_code,
                                      PSecPkgInfoW package_info);

 private:
  struct PackageQuery {
    std::wstring expected_package;
    SECURITY_STATUS response_code;
    PSecPkgInfoW package_info;
  };

  
  
  
  std::list<PackageQuery> expected_package_queries_;

  
  std::set<PSecPkgInfoW> expected_freed_packages_;
};

}  

#endif  
