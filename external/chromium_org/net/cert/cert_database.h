// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_CERT_CERT_DATABASE_H_
#define NET_CERT_CERT_DATABASE_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/net_export.h"
#include "net/cert/x509_certificate.h"

template <typename T> struct DefaultSingletonTraits;
template <class ObserverType> class ObserverListThreadSafe;

namespace net {



class NET_EXPORT CertDatabase {
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

  
  static CertDatabase* GetInstance();

  
  
  int CheckUserCert(X509Certificate* cert);

  
  
  
  int AddUserCert(X509Certificate* cert);

  
  
  
  void AddObserver(Observer* observer);

  
  
  void RemoveObserver(Observer* observer);

#if defined(OS_MACOSX) && !defined(OS_IOS)
  
  
  
  void SetMessageLoopForKeychainEvents();
#endif

#if defined(OS_ANDROID)
  
  
  void OnAndroidKeyChainChanged();
#endif

 private:
  friend struct DefaultSingletonTraits<CertDatabase>;

  CertDatabase();
  ~CertDatabase();

  
  void NotifyObserversOfCertAdded(const X509Certificate* cert);
  void NotifyObserversOfCertRemoved(const X509Certificate* cert);
  void NotifyObserversOfCACertChanged(const X509Certificate* cert);

  const scoped_refptr<ObserverListThreadSafe<Observer> > observer_list_;

#if defined(USE_NSS) || (defined(OS_MACOSX) && !defined(OS_IOS))
  class Notifier;
  friend class Notifier;
  scoped_ptr<Notifier> notifier_;
#endif

  DISALLOW_COPY_AND_ASSIGN(CertDatabase);
};

}  

#endif  
