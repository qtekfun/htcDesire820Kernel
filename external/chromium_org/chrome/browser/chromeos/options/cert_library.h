// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_OPTIONS_CERT_LIBRARY_H_
#define CHROME_BROWSER_CHROMEOS_OPTIONS_CERT_LIBRARY_H_

#include <string>

#include "base/strings/string16.h"
#include "chromeos/cert_loader.h"
#include "net/cert/x509_certificate.h"

namespace chromeos {

class CertNameComparator;

class CertLibrary : public CertLoader::Observer {
 public:
  class Observer {
   public:
    virtual ~Observer() {}

    
    
    virtual void OnCertificatesLoaded(bool initial_load) = 0;

   protected:
    Observer() {}

   private:
    DISALLOW_COPY_AND_ASSIGN(Observer);
  };

  enum CertType {
    CERT_TYPE_DEFAULT,
    CERT_TYPE_USER,
    CERT_TYPE_SERVER,
    CERT_TYPE_SERVER_CA
  };

  
  static void Initialize();
  static void Shutdown();
  static CertLibrary* Get();
  static bool IsInitialized();

  
  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

  
  bool CertificatesLoading() const;

  
  bool CertificatesLoaded() const;

  
  bool IsHardwareBacked() const;

  
  int NumCertificates(CertType type) const;

  
  base::string16 GetCertDisplayStringAt(CertType type, int index) const;
  std::string GetCertPEMAt(CertType type, int index) const;
  std::string GetCertPkcs11IdAt(CertType type, int index) const;
  bool IsCertHardwareBackedAt(CertType type, int index) const;

  
  
  
  
  
  int GetCertIndexByPEM(CertType type, const std::string& pem_encoded) const;
  
  
  int GetCertIndexByPkcs11Id(CertType type, const std::string& pkcs11_id) const;

  
  virtual void OnCertificatesLoaded(const net::CertificateList&,
                                    bool initial_load) OVERRIDE;

 private:
  CertLibrary();
  virtual ~CertLibrary();

  net::X509Certificate* GetCertificateAt(CertType type, int index) const;
  const net::CertificateList& GetCertificateListForType(CertType type) const;

  ObserverList<CertLibrary::Observer> observer_list_;

  
  net::CertificateList certs_;
  net::CertificateList user_certs_;
  net::CertificateList server_certs_;
  net::CertificateList server_ca_certs_;

  DISALLOW_COPY_AND_ASSIGN(CertLibrary);
};

}  

#endif  
