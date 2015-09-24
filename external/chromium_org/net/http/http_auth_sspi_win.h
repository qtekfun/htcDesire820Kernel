// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_HTTP_HTTP_AUTH_SSPI_WIN_H_
#define NET_HTTP_HTTP_AUTH_SSPI_WIN_H_

#define SECURITY_WIN32 1
#include <windows.h>
#include <security.h>

#include <string>

#include "base/strings/string16.h"
#include "net/base/net_export.h"
#include "net/http/http_auth.h"

namespace net {

class SSPILibrary {
 public:
  virtual ~SSPILibrary() {}

  virtual SECURITY_STATUS AcquireCredentialsHandle(LPWSTR pszPrincipal,
                                                   LPWSTR pszPackage,
                                                   unsigned long fCredentialUse,
                                                   void* pvLogonId,
                                                   void* pvAuthData,
                                                   SEC_GET_KEY_FN pGetKeyFn,
                                                   void* pvGetKeyArgument,
                                                   PCredHandle phCredential,
                                                   PTimeStamp ptsExpiry) = 0;

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
                                                    PTimeStamp ptsExpiry) = 0;

  virtual SECURITY_STATUS QuerySecurityPackageInfo(LPWSTR pszPackageName,
                                                   PSecPkgInfoW *pkgInfo) = 0;

  virtual SECURITY_STATUS FreeCredentialsHandle(PCredHandle phCredential) = 0;

  virtual SECURITY_STATUS DeleteSecurityContext(PCtxtHandle phContext) = 0;

  virtual SECURITY_STATUS FreeContextBuffer(PVOID pvContextBuffer) = 0;
};

class SSPILibraryDefault : public SSPILibrary {
 public:
  SSPILibraryDefault() {}
  virtual ~SSPILibraryDefault() {}

  virtual SECURITY_STATUS AcquireCredentialsHandle(LPWSTR pszPrincipal,
                                                   LPWSTR pszPackage,
                                                   unsigned long fCredentialUse,
                                                   void* pvLogonId,
                                                   void* pvAuthData,
                                                   SEC_GET_KEY_FN pGetKeyFn,
                                                   void* pvGetKeyArgument,
                                                   PCredHandle phCredential,
                                                   PTimeStamp ptsExpiry) {
    return ::AcquireCredentialsHandle(pszPrincipal, pszPackage, fCredentialUse,
                                      pvLogonId, pvAuthData, pGetKeyFn,
                                      pvGetKeyArgument, phCredential,
                                      ptsExpiry);
  }

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
                                                    PTimeStamp ptsExpiry) {
    return ::InitializeSecurityContext(phCredential, phContext, pszTargetName,
                                       fContextReq, Reserved1, TargetDataRep,
                                       pInput, Reserved2, phNewContext, pOutput,
                                       contextAttr, ptsExpiry);
  }

  virtual SECURITY_STATUS QuerySecurityPackageInfo(LPWSTR pszPackageName,
                                                   PSecPkgInfoW *pkgInfo) {
    return ::QuerySecurityPackageInfo(pszPackageName, pkgInfo);
  }

  virtual SECURITY_STATUS FreeCredentialsHandle(PCredHandle phCredential) {
    return ::FreeCredentialsHandle(phCredential);
  }

  virtual SECURITY_STATUS DeleteSecurityContext(PCtxtHandle phContext) {
    return ::DeleteSecurityContext(phContext);
  }

  virtual SECURITY_STATUS FreeContextBuffer(PVOID pvContextBuffer) {
    return ::FreeContextBuffer(pvContextBuffer);
  }
};

class NET_EXPORT_PRIVATE HttpAuthSSPI {
 public:
  HttpAuthSSPI(SSPILibrary* sspi_library,
               const std::string& scheme,
               const SEC_WCHAR* security_package,
               ULONG max_token_length);
  ~HttpAuthSSPI();

  bool NeedsIdentity() const;

  bool AllowsExplicitCredentials() const;

  HttpAuth::AuthorizationResult ParseChallenge(
      HttpAuth::ChallengeTokenizer* tok);

  
  
  
  
  
  
  
  int GenerateAuthToken(const AuthCredentials* credentials,
                        const std::string& spn,
                        std::string* auth_token);

  
  
  
  void Delegate();

 private:
  int OnFirstRound(const AuthCredentials* credentials);

  int GetNextSecurityToken(
      const std::string& spn,
      const void* in_token,
      int in_token_len,
      void** out_token,
      int* out_token_len);

  void ResetSecurityContext();

  SSPILibrary* library_;
  std::string scheme_;
  const SEC_WCHAR* security_package_;
  std::string decoded_server_auth_token_;
  ULONG max_token_length_;
  CredHandle cred_;
  CtxtHandle ctxt_;
  bool can_delegate_;
};

NET_EXPORT_PRIVATE void SplitDomainAndUser(const base::string16& combined,
                                           base::string16* domain,
                                           base::string16* user);

NET_EXPORT_PRIVATE int DetermineMaxTokenLength(SSPILibrary* library,
                                               const std::wstring& package,
                                               ULONG* max_token_length);

}  

#endif  
