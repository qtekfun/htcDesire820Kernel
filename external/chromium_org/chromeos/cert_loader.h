// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_CERT_LOADER_H_
#define CHROMEOS_CERT_LOADER_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list_threadsafe.h"
#include "base/threading/thread_checker.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/dbus_method_call_status.h"
#include "chromeos/login/login_state.h"
#include "net/cert/cert_database.h"
#include "net/cert/x509_certificate.h"

namespace base {
class SequencedTaskRunner;
class TaskRunner;
}

namespace crypto {
class SymmetricKey;
}

namespace chromeos {

class CHROMEOS_EXPORT CertLoader : public net::CertDatabase::Observer,
                                   public LoginState::Observer {
 public:
  class Observer {
   public:
    virtual ~Observer() {}

    
    
    
    virtual void OnCertificatesLoaded(const net::CertificateList& cert_list,
                                      bool initial_load) = 0;

   protected:
    Observer() {}

   private:
    DISALLOW_COPY_AND_ASSIGN(Observer);
  };

  
  static void Initialize();

  
  static void Shutdown();

  
  static CertLoader* Get();

  
  static bool IsInitialized();

  static std::string GetPkcs11IdForCert(const net::X509Certificate& cert);

  
  
  
  void InitializeTPMForTest();

  
  
  
  
  void SetCryptoTaskRunner(
      const scoped_refptr<base::SequencedTaskRunner>& crypto_task_runner);

  
  
  void SetSlowTaskRunnerForTest(
      const scoped_refptr<base::TaskRunner>& task_runner);

  void AddObserver(CertLoader::Observer* observer);
  void RemoveObserver(CertLoader::Observer* observer);

  
  bool CertificatesLoading() const;

  
  bool IsHardwareBacked() const;

  bool certificates_loaded() const { return certificates_loaded_; }

  
  
  const std::string& tpm_token_name() const { return tpm_token_name_; }
  int tpm_token_slot_id() const { return tpm_token_slot_id_; }
  const std::string& tpm_user_pin() const { return tpm_user_pin_; }

  
  const net::CertificateList& cert_list() const { return cert_list_; }

 private:
  CertLoader();
  virtual ~CertLoader();

  void MaybeRequestCertificates();

  
  
  void InitializeTokenAndLoadCertificates();
  void RetryTokenInitializationLater();
  void OnPersistentNSSDBOpened();
  void OnTpmIsEnabled(DBusMethodCallStatus call_status,
                      bool tpm_is_enabled);
  void OnPkcs11IsTpmTokenReady(DBusMethodCallStatus call_status,
                               bool is_tpm_token_ready);
  void OnPkcs11GetTpmTokenInfo(DBusMethodCallStatus call_status,
                               const std::string& token_name,
                               const std::string& user_pin,
                               int token_slot_id);
  void OnTPMTokenInitialized(bool success);

  
  

  
  void StartLoadCertificates();

  
  
  void LoadCertificates();

  
  void UpdateCertificates(net::CertificateList* cert_list);

  void NotifyCertificatesLoaded(bool initial_load);

  
  virtual void OnCACertChanged(const net::X509Certificate* cert) OVERRIDE;
  virtual void OnCertAdded(const net::X509Certificate* cert) OVERRIDE;
  virtual void OnCertRemoved(const net::X509Certificate* cert) OVERRIDE;

  
  virtual void LoggedInStateChanged() OVERRIDE;

  bool initialize_tpm_for_test_;

  ObserverList<Observer> observers_;

  bool certificates_requested_;
  bool certificates_loaded_;
  bool certificates_update_required_;
  bool certificates_update_running_;

  
  
  enum TPMTokenState {
    TPM_STATE_UNKNOWN,
    TPM_DB_OPENED,
    TPM_DISABLED,
    TPM_ENABLED,
    TPM_TOKEN_READY,
    TPM_TOKEN_INFO_RECEIVED,
    TPM_TOKEN_INITIALIZED,
  };
  TPMTokenState tpm_token_state_;

  
  
  base::TimeDelta tpm_request_delay_;

  
  std::string tpm_token_name_;
  int tpm_token_slot_id_;
  std::string tpm_user_pin_;

  
  net::CertificateList cert_list_;

  base::ThreadChecker thread_checker_;

  
  scoped_refptr<base::SequencedTaskRunner> crypto_task_runner_;

  
  scoped_refptr<base::TaskRunner> slow_task_runner_for_test_;

  
  
  base::WeakPtrFactory<CertLoader> initialize_token_factory_;

  
  
  base::WeakPtrFactory<CertLoader> update_certificates_factory_;

  DISALLOW_COPY_AND_ASSIGN(CertLoader);
};

}  

#endif  
