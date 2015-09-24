// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_CERT_NSS_CERT_DATABASE_H_
#define NET_CERT_NSS_CERT_DATABASE_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/strings/string16.h"
#include "crypto/scoped_nss_types.h"
#include "net/base/net_export.h"
#include "net/cert/cert_type.h"
#include "net/cert/x509_certificate.h"

template <typename T> struct DefaultSingletonTraits;
template <class ObserverType> class ObserverListThreadSafe;

namespace net {

class CryptoModule;
typedef std::vector<scoped_refptr<CryptoModule> > CryptoModuleList;

class NET_EXPORT NSSCertDatabase {
 public:

  class NET_EXPORT Observer {
   public:
    virtual ~Observer() {}

    
    
    
    virtual void OnCertAdded(const X509Certificate* cert) {}

    
    virtual void OnCertRemoved(const X509Certificate* cert) {}

    
    
    
    virtual void OnCACertChanged(const X509Certificate* cert) {}

   protected:
    Observer() {}

   private:
    DISALLOW_COPY_AND_ASSIGN(Observer);
  };

  
  struct NET_EXPORT ImportCertFailure {
   public:
    ImportCertFailure(const scoped_refptr<X509Certificate>& cert, int err);
    ~ImportCertFailure();

    scoped_refptr<X509Certificate> certificate;
    int net_error;
  };
  typedef std::vector<ImportCertFailure> ImportCertFailureList;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  typedef uint32 TrustBits;
  enum {
    TRUST_DEFAULT         =      0,
    TRUSTED_SSL           = 1 << 0,
    TRUSTED_EMAIL         = 1 << 1,
    TRUSTED_OBJ_SIGN      = 1 << 2,
    DISTRUSTED_SSL        = 1 << 3,
    DISTRUSTED_EMAIL      = 1 << 4,
    DISTRUSTED_OBJ_SIGN   = 1 << 5,
  };

  static NSSCertDatabase* GetInstance();

  
  
  void ListCerts(CertificateList* certs);

  
  crypto::ScopedPK11Slot GetPublicSlot() const;

  
  crypto::ScopedPK11Slot GetPrivateSlot() const;

  
  
  
  
  CryptoModule* GetPublicModule() const;

  
  
  
  
  CryptoModule* GetPrivateModule() const;

  
  
  
  void ListModules(CryptoModuleList* modules, bool need_rw) const;

  
  
  
  
  
  
  int ImportFromPKCS12(CryptoModule* module,
                       const std::string& data,
                       const base::string16& password,
                       bool is_extractable,
                       CertificateList* imported_certs);

  
  
  
  int ExportToPKCS12(const CertificateList& certs,
                     const base::string16& password,
                     std::string* output) const;

  
  
  
  
  X509Certificate* FindRootInList(const CertificateList& certificates) const;

  
  
  
  
  
  
  
  bool ImportCACerts(const CertificateList& certificates,
                     TrustBits trust_bits,
                     ImportCertFailureList* not_imported);

  
  
  
  
  
  
  
  
  
  
  bool ImportServerCert(const CertificateList& certificates,
                        TrustBits trust_bits,
                        ImportCertFailureList* not_imported);

  
  TrustBits GetCertTrust(const X509Certificate* cert, CertType type) const;

  
  
  
  bool IsUntrusted(const X509Certificate* cert) const;

  
  
  bool SetCertTrust(const X509Certificate* cert,
                    CertType type,
                    TrustBits trust_bits);

  
  
  
  bool DeleteCertAndKey(const X509Certificate* cert);

  
  bool IsReadOnly(const X509Certificate* cert) const;

  
  bool IsHardwareBacked(const X509Certificate* cert) const;

  
  
  
  void AddObserver(Observer* observer);

  
  
  void RemoveObserver(Observer* observer);

 private:
  friend struct DefaultSingletonTraits<NSSCertDatabase>;

  NSSCertDatabase();
  ~NSSCertDatabase();

  
  void NotifyObserversOfCertAdded(const X509Certificate* cert);
  void NotifyObserversOfCertRemoved(const X509Certificate* cert);
  void NotifyObserversOfCACertChanged(const X509Certificate* cert);

  const scoped_refptr<ObserverListThreadSafe<Observer> > observer_list_;

  DISALLOW_COPY_AND_ASSIGN(NSSCertDatabase);
};

}  

#endif  
