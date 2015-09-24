// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_CERT_X509_CERTIFICATE_H_
#define NET_CERT_X509_CERTIFICATE_H_

#include <string.h>

#include <string>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/strings/string_piece.h"
#include "base/time/time.h"
#include "net/base/net_export.h"
#include "net/cert/cert_type.h"
#include "net/cert/x509_cert_types.h"

#if defined(OS_WIN)
#include <windows.h>
#include <wincrypt.h>
#elif defined(OS_MACOSX)
#include <CoreFoundation/CFArray.h>
#include <Security/SecBase.h>

#elif defined(USE_OPENSSL)
typedef struct x509_st X509;
typedef struct x509_store_st X509_STORE;
#elif defined(USE_NSS)
struct CERTCertificateStr;
#endif

class Pickle;
class PickleIterator;

namespace net {

class CRLSet;
class CertVerifyResult;

typedef std::vector<scoped_refptr<X509Certificate> > CertificateList;

class NET_EXPORT X509Certificate
    : public base::RefCountedThreadSafe<X509Certificate> {
 public:
  
  
  
#if defined(OS_WIN)
  typedef PCCERT_CONTEXT OSCertHandle;
#elif defined(OS_MACOSX)
  typedef SecCertificateRef OSCertHandle;
#elif defined(USE_OPENSSL)
  typedef X509* OSCertHandle;
#elif defined(USE_NSS)
  typedef struct CERTCertificateStr* OSCertHandle;
#else
  
  typedef void* OSCertHandle;
#endif

  typedef std::vector<OSCertHandle> OSCertHandles;

  enum PublicKeyType {
    kPublicKeyTypeUnknown,
    kPublicKeyTypeRSA,
    kPublicKeyTypeDSA,
    kPublicKeyTypeECDSA,
    kPublicKeyTypeDH,
    kPublicKeyTypeECDH
  };

  
  class NET_EXPORT LessThan {
   public:
    bool operator()(const scoped_refptr<X509Certificate>& lhs,
                    const scoped_refptr<X509Certificate>& rhs) const;
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

    
    
    
    
    
    PICKLETYPE_CERTIFICATE_CHAIN_V2,

    
    
    
    
    
    PICKLETYPE_CERTIFICATE_CHAIN_V3,
  };

  
  
  X509Certificate(const std::string& subject, const std::string& issuer,
                  base::Time start_date, base::Time expiration_date);

  
  
  
  static X509Certificate* CreateFromHandle(OSCertHandle cert_handle,
                                           const OSCertHandles& intermediates);

  
  
  
  
  
  static X509Certificate* CreateFromDERCertChain(
      const std::vector<base::StringPiece>& der_certs);

  
  
  
  
  static X509Certificate* CreateFromBytes(const char* data, int length);

#if defined(USE_NSS)
  
  
  
  
  
  
  
  static X509Certificate* CreateFromBytesWithNickname(const char* data,
                                                      int length,
                                                      const char* nickname);

  
  
  
  std::string GetDefaultNickname(CertType type) const;
#endif

  
  
  
  
  
  
  static X509Certificate* CreateFromPickle(const Pickle& pickle,
                                           PickleIterator* pickle_iter,
                                           PickleType type);

  
  
  
  
  static CertificateList CreateCertificateListFromBytes(const char* data,
                                                        int length,
                                                        int format);

  
  void Persist(Pickle* pickle);

  
  const std::string& serial_number() const { return serial_number_; }

  
  
  
  const CertPrincipal& subject() const { return subject_; }

  
  const CertPrincipal& issuer() const { return issuer_; }

  
  
  
  
  
  const base::Time& valid_start() const { return valid_start_; }
  const base::Time& valid_expiry() const { return valid_expiry_; }

  
  const SHA1HashValue& fingerprint() const { return fingerprint_; }

  
  const SHA1HashValue& ca_fingerprint() const {
    return ca_fingerprint_;
  }

  
  
  
  
  void GetDNSNames(std::vector<std::string>* dns_names) const;

  
  
  
  
  void GetSubjectAltName(std::vector<std::string>* dns_names,
                         std::vector<std::string>* ip_addrs) const;

  
  
  bool HasExpired() const;

  
  bool Equals(const X509Certificate* other) const;

  
  
  
  const OSCertHandles& GetIntermediateCertificates() const {
    return intermediate_ca_certs_;
  }

#if defined(OS_MACOSX)
  
  bool SupportsSSLClientAuth() const;

  
  
  
  
  
  CFArrayRef CreateOSCertChainForCert() const;
#endif

  
  
  bool IsIssuedByEncoded(const std::vector<std::string>& valid_issuers);

#if defined(OS_WIN)
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  PCCERT_CONTEXT CreateOSCertChainForCert() const;
#endif

#if defined(USE_OPENSSL)
  
  
  static X509_STORE* cert_store();
#endif

  
  
  
  
  
  
  bool VerifyNameMatch(const std::string& hostname,
                       bool* common_name_fallback_used) const;

  
  
  static bool GetDEREncoded(OSCertHandle cert_handle,
                            std::string* der_encoded);

  
  // value is true, then the PEM encoded certificate is written to
  
  static bool GetPEMEncodedFromDER(const std::string& der_encoded,
                                   std::string* pem_encoded);

  
  // true, then the PEM encoded certificate is written to |pem_encoded|.
  static bool GetPEMEncoded(OSCertHandle cert_handle,
                            std::string* pem_encoded);

  
  
  
  
  
  bool GetPEMEncodedChain(std::vector<std::string>* pem_encoded) const;

  
  
  
  static void GetPublicKeyInfo(OSCertHandle cert_handle,
                               size_t* size_bits,
                               PublicKeyType* type);

  
  
  
  
  
  OSCertHandle os_cert_handle() const { return cert_handle_; }

  
  static bool IsSameOSCert(OSCertHandle a, OSCertHandle b);

  
  
  static OSCertHandle CreateOSCertHandleFromBytes(const char* data,
                                                  int length);

#if defined(USE_NSS)
  
  
  
  static OSCertHandle CreateOSCertHandleFromBytesWithNickname(
      const char* data,
      int length,
      const char* nickname);
#endif

  
  
  static OSCertHandles CreateOSCertHandlesFromBytes(
      const char* data,
      int length,
      Format format);

  
  static OSCertHandle DupOSCertHandle(OSCertHandle cert_handle);

  
  static void FreeOSCertHandle(OSCertHandle cert_handle);

  
  
  static SHA1HashValue CalculateFingerprint(OSCertHandle cert_handle);

  
  
  static SHA1HashValue CalculateCAFingerprint(
      const OSCertHandles& intermediates);

 private:
  friend class base::RefCountedThreadSafe<X509Certificate>;
  friend class TestRootCerts;  

  FRIEND_TEST_ALL_PREFIXES(X509CertificateNameVerifyTest, VerifyHostname);
  FRIEND_TEST_ALL_PREFIXES(X509CertificateTest, SerialNumbers);

  
  
  X509Certificate(OSCertHandle cert_handle,
                  const OSCertHandles& intermediates);

  ~X509Certificate();

  
  void Initialize();

#if defined(USE_OPENSSL)
  
  
  static void ResetCertStore();
#endif

  
  
  
  
  
  
  
  
  
  
  
  
  static bool VerifyHostname(const std::string& hostname,
                             const std::string& cert_common_name,
                             const std::vector<std::string>& cert_san_dns_names,
                             const std::vector<std::string>& cert_san_ip_addrs,
                             bool* common_name_fallback_used);

  
  
  
  
  
  
  
  static OSCertHandle ReadOSCertHandleFromPickle(PickleIterator* pickle_iter);

  
  
  static bool WriteOSCertHandleToPickle(OSCertHandle handle, Pickle* pickle);

  
  CertPrincipal subject_;

  
  CertPrincipal issuer_;

  
  base::Time valid_start_;

  
  base::Time valid_expiry_;

  
  SHA1HashValue fingerprint_;

  
  SHA1HashValue ca_fingerprint_;

  
  std::string serial_number_;

  
  OSCertHandle cert_handle_;

  
  
  OSCertHandles intermediate_ca_certs_;

#if defined(USE_NSS)
  
  
  
  
  std::string default_nickname_;
#endif

  DISALLOW_COPY_AND_ASSIGN(X509Certificate);
};

}  

#endif  
