// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_CERT_DATABASE_H_
#define NET_BASE_CERT_DATABASE_H_
#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/string16.h"
#include "net/base/cert_type.h"

namespace net {

class CryptoModule;
typedef std::vector<scoped_refptr<CryptoModule> > CryptoModuleList;
class X509Certificate;
typedef std::vector<scoped_refptr<X509Certificate> > CertificateList;




class CertDatabase {
 public:

  
  
  
  
  
  class Observer {
   public:
    virtual ~Observer() {}

    
    
    virtual void OnUserCertAdded(const X509Certificate* cert) {}

    
    
    virtual void OnCertTrustChanged(const X509Certificate* cert) {}

   protected:
    Observer() {}

   private:
    DISALLOW_COPY_AND_ASSIGN(Observer);
  };

  
  struct ImportCertFailure {
   public:
    ImportCertFailure(X509Certificate* cert, int err);
    ~ImportCertFailure();

    scoped_refptr<X509Certificate> certificate;
    int net_error;
  };
  typedef std::vector<ImportCertFailure> ImportCertFailureList;

  
  
  
  
  
  
  
  
  
  enum {
    UNTRUSTED        =      0,
    TRUSTED_SSL      = 1 << 0,
    TRUSTED_EMAIL    = 1 << 1,
    TRUSTED_OBJ_SIGN = 1 << 2,
  };

  CertDatabase();

  
  
  int CheckUserCert(X509Certificate* cert);

  
  
  
  int AddUserCert(X509Certificate* cert);

#if defined(USE_NSS) || defined(USE_OPENSSL)
  
  
  void ListCerts(CertificateList* certs);

  
  
  CryptoModule* GetPublicModule() const;

  
  
  CryptoModule* GetPrivateModule() const;

  
  
  void ListModules(CryptoModuleList* modules, bool need_rw) const;

  
  
  
  int ImportFromPKCS12(CryptoModule* module,
                       const std::string& data,
                       const string16& password);

  
  
  
  int ExportToPKCS12(const CertificateList& certs, const string16& password,
                     std::string* output) const;

  
  
  
  
  X509Certificate* FindRootInList(const CertificateList& certificates) const;

  
  
  
  
  
  
  
  bool ImportCACerts(const CertificateList& certificates,
                     unsigned int trust_bits,
                     ImportCertFailureList* not_imported);

  
  
  
  
  
  
  
  
  bool ImportServerCert(const CertificateList& certificates,
                        ImportCertFailureList* not_imported);

  
  unsigned int GetCertTrust(const X509Certificate* cert, CertType type) const;

  
  
  bool SetCertTrust(const X509Certificate* cert,
                    CertType type,
                    unsigned int trust_bits);

  
  
  
  bool DeleteCertAndKey(const X509Certificate* cert);

  
  bool IsReadOnly(const X509Certificate* cert) const;
#endif

  
  
  
  static void AddObserver(Observer* observer);

  
  
  static void RemoveObserver(Observer* observer);

 private:
  
  static void NotifyObserversOfUserCertAdded(const X509Certificate* cert);
  static void NotifyObserversOfCertTrustChanged(const X509Certificate* cert);

  DISALLOW_COPY_AND_ASSIGN(CertDatabase);
};

}  

#endif  
