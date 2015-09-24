// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_X509_CERTIFICATE_H_
#define NET_BASE_X509_CERTIFICATE_H_
#pragma once

#include <string.h>

#include <string>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/string_piece.h"
#include "base/time.h"
#include "net/base/net_export.h"
#include "net/base/x509_cert_types.h"

#if defined(OS_WIN)
#include <windows.h>
#include <wincrypt.h>
#elif defined(OS_MACOSX)
#include <CoreFoundation/CFArray.h>
#include <Security/SecBase.h>

#include "base/synchronization/lock.h"
#elif defined(USE_OPENSSL)
struct x509_st;
typedef struct x509_store_st X509_STORE;
#elif defined(USE_NSS)
struct CERTCertificateStr;
#endif

class Pickle;

namespace crypto {
class StringPiece;
class RSAPrivateKey;
}  

namespace net {

class CertVerifyResult;

typedef std::vector<scoped_refptr<X509Certificate> > CertificateList;

class NET_EXPORT X509Certificate : public base::RefCountedThreadSafe<X509Certificate> {
 public:
  
  
  
#if defined(OS_WIN)
  typedef PCCERT_CONTEXT OSCertHandle;
#elif defined(OS_MACOSX)
  typedef SecCertificateRef OSCertHandle;
#elif defined(USE_OPENSSL)
  typedef struct x509_st* OSCertHandle;
#elif defined(USE_NSS)
  typedef struct CERTCertificateStr* OSCertHandle;
#else
  
  typedef void* OSCertHandle;
#endif

  typedef std::vector<OSCertHandle> OSCertHandles;

  
  class LessThan {
   public:
    bool operator() (X509Certificate* lhs,  X509Certificate* rhs) const;
  };

  
  
  enum Source {
    SOURCE_UNUSED = 0,            
    SOURCE_LONE_CERT_IMPORT = 1,  
                                  
    SOURCE_FROM_CACHE = 2,        
                                  
                                  
    SOURCE_FROM_NETWORK = 3,      
  };

  enum VerifyFlags {
    VERIFY_REV_CHECKING_ENABLED = 1 << 0,
    VERIFY_EV_CERT = 1 << 1,
  };

  enum Format {
    
    
    
    FORMAT_SINGLE_CERTIFICATE = 1 << 0,

    
    
    
    FORMAT_PEM_CERT_SEQUENCE = 1 << 1,

    
    
    
    
    FORMAT_PKCS7 = 1 << 2,

    
    FORMAT_AUTO = FORMAT_SINGLE_CERTIFICATE | FORMAT_PEM_CERT_SEQUENCE |
                  FORMAT_PKCS7,
  };

  enum PickleType {
    
    
    PICKLETYPE_SINGLE_CERTIFICATE,

    
    
    
    PICKLETYPE_CERTIFICATE_CHAIN,
  };

  
  
  X509Certificate(const std::string& subject, const std::string& issuer,
                  base::Time start_date, base::Time expiration_date);

  
  
  
  
  
  
  
  static scoped_refptr<X509Certificate> CreateFromHandle(OSCertHandle cert_handle,
                                           Source source,
                                           const OSCertHandles& intermediates);

  
  
  
  
  
  
  static scoped_refptr<X509Certificate> CreateFromDERCertChain(
      const std::vector<base::StringPiece>& der_certs);

  
  
  
  
  static scoped_refptr<X509Certificate> CreateFromBytes(const char* data, int length);

  
  
  
  
  
  
  static scoped_refptr<X509Certificate> CreateFromPickle(const Pickle& pickle,
                                           void** pickle_iter,
                                           PickleType type);

  
  
  
  
  static CertificateList CreateCertificateListFromBytes(const char* data,
                                                        int length,
                                                        int format);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static scoped_refptr<X509Certificate> CreateSelfSigned(crypto::RSAPrivateKey* key,
                                           const std::string& subject,
                                           uint32 serial_number,
                                           base::TimeDelta valid_duration);

  
  void Persist(Pickle* pickle);

  
  
  
  const CertPrincipal& subject() const { return subject_; }

  
  const CertPrincipal& issuer() const { return issuer_; }

  
  
  
  
  
  const base::Time& valid_start() const { return valid_start_; }
  const base::Time& valid_expiry() const { return valid_expiry_; }

  
  const SHA1Fingerprint& fingerprint() const { return fingerprint_; }

  
  
  
  
  void GetDNSNames(std::vector<std::string>* dns_names) const;

  
  
  bool HasExpired() const;

  
  bool Equals(const X509Certificate* other) const;

  
  
  
  const OSCertHandles& GetIntermediateCertificates() const {
    return intermediate_ca_certs_;
  }

  
  bool HasIntermediateCertificate(OSCertHandle cert);

  
  bool HasIntermediateCertificates(const OSCertHandles& certs);

#if defined(OS_MACOSX)
  
  bool SupportsSSLClientAuth() const;

  
  bool IsIssuedBy(const std::vector<CertPrincipal>& valid_issuers);

  
  static OSStatus CreateSSLClientPolicy(SecPolicyRef* outPolicy);

  
  
  
  
  
  
  static bool GetSSLClientCertificates(
      const std::string& server_domain,
      const std::vector<CertPrincipal>& valid_issuers,
      CertificateList* certs);

  
  CFArrayRef CreateClientCertificateChain() const;
#endif

#if defined(OS_WIN)
  
  
  
  
  
  
  
  static HCERTSTORE cert_store();
#endif

#if defined(USE_OPENSSL)
  
  
  static X509_STORE* cert_store();
#endif

#if defined(ANDROID)
  
  
  void GetChainDEREncodedBytes(std::vector<std::string>* chain_bytes) const;
#endif

  
  
  
  
  
  
  
  
  
  
  
  
  
  int Verify(const std::string& hostname,
             int flags,
             CertVerifyResult* verify_result) const;

  
  
  
  
  
  
  
  
  bool VerifyNameMatch(const std::string& hostname) const;

  
  
  // The content of the DER encoded certificate is written to |encoded|.
  bool GetDEREncoded(std::string* encoded);

  OSCertHandle os_cert_handle() const { return cert_handle_; }

  
  static bool IsSameOSCert(OSCertHandle a, OSCertHandle b);

  
  
  static OSCertHandle CreateOSCertHandleFromBytes(const char* data,
                                                  int length);

  
  
  static OSCertHandles CreateOSCertHandlesFromBytes(
      const char* data, int length, Format format);

  
  static OSCertHandle DupOSCertHandle(OSCertHandle cert_handle);

  
  static void FreeOSCertHandle(OSCertHandle cert_handle);

 private:
  friend class base::RefCountedThreadSafe<X509Certificate>;
  friend class TestRootCerts;  
  FRIEND_TEST_ALL_PREFIXES(X509CertificateTest, Cache);
  FRIEND_TEST_ALL_PREFIXES(X509CertificateTest, IntermediateCertificates);
  FRIEND_TEST_ALL_PREFIXES(X509CertificateTest, SerialNumbers);
  FRIEND_TEST_ALL_PREFIXES(X509CertificateNameVerifyTest, VerifyHostname);

  
  
  X509Certificate(OSCertHandle cert_handle, Source source,
                  const OSCertHandles& intermediates);

  ~X509Certificate();

  
  void Initialize();

#if defined(OS_WIN)
  bool CheckEV(PCCERT_CHAIN_CONTEXT chain_context,
               const char* policy_oid) const;
  static bool IsIssuedByKnownRoot(PCCERT_CHAIN_CONTEXT chain_context);
#endif
#if defined(OS_MACOSX)
  static bool IsIssuedByKnownRoot(CFArrayRef chain);
#endif
  bool VerifyEV() const;

#if defined(USE_OPENSSL)
  
  
  static void ResetCertStore();
#endif

  
  
  static SHA1Fingerprint CalculateFingerprint(OSCertHandle cert_handle);

  
  
  
  
  
  
  
  static bool VerifyHostname(const std::string& hostname,
                             const std::vector<std::string>& cert_names);

  
  
  
  
  
  
  const std::string& serial_number() const { return serial_number_; }

  
  bool IsBlacklisted() const;

  
  
  static bool IsPublicKeyBlacklisted(
      const std::vector<SHA1Fingerprint>& public_key_hashes);

  
  
  static bool IsSHA1HashInSortedArray(const SHA1Fingerprint& hash,
                                      const uint8* array,
                                      size_t array_byte_len);

  
  
  
  
  
  static OSCertHandle ReadCertHandleFromPickle(const Pickle& pickle,
                                               void** pickle_iter);

  
  static bool WriteCertHandleToPickle(OSCertHandle handle, Pickle* pickle);

#ifdef ANDROID
#if defined(USE_OPENSSL)
  
  
  
  static std::string GetDEREncodedBytes(OSCertHandle handle);
#endif
#endif

  
  CertPrincipal subject_;

  
  CertPrincipal issuer_;

  
  base::Time valid_start_;

  
  base::Time valid_expiry_;

  
  SHA1Fingerprint fingerprint_;

  
  std::string serial_number_;

  
  OSCertHandle cert_handle_;

  
  
  OSCertHandles intermediate_ca_certs_;

#if defined(OS_MACOSX)
  
  
  mutable base::Lock verification_lock_;
#endif

  
  Source source_;

  DISALLOW_COPY_AND_ASSIGN(X509Certificate);
};

}  

#endif  
