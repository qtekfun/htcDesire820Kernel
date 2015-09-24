// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_SETTINGS_DEVICE_SETTINGS_SERVICE_H_
#define CHROME_BROWSER_CHROMEOS_SETTINGS_DEVICE_SETTINGS_SERVICE_H_

#include <deque>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "chromeos/cert_loader.h"
#include "chromeos/dbus/session_manager_client.h"
#include "components/policy/core/common/cloud/cloud_policy_validator.h"

namespace crypto {
class RSAPrivateKey;
}

namespace enterprise_management {
class ChromeDeviceSettingsProto;
class PolicyData;
class PolicyFetchResponse;
}

namespace chromeos {

class OwnerKeyUtil;
class SessionManagerOperation;

class OwnerKey : public base::RefCountedThreadSafe<OwnerKey> {
 public:
  OwnerKey(scoped_ptr<std::vector<uint8> > public_key,
           scoped_ptr<crypto::RSAPrivateKey> private_key);

  const std::vector<uint8>* public_key() {
    return public_key_.get();
  }
  crypto::RSAPrivateKey* private_key() {
    return private_key_.get();
  }

 private:
  friend class base::RefCountedThreadSafe<OwnerKey>;
  ~OwnerKey();

  scoped_ptr<std::vector<uint8> > public_key_;
  scoped_ptr<crypto::RSAPrivateKey> private_key_;

  DISALLOW_COPY_AND_ASSIGN(OwnerKey);
};

class DeviceSettingsService : public SessionManagerClient::Observer,
                              public CertLoader::Observer {
 public:
  
  enum OwnershipStatus {
    
    OWNERSHIP_UNKNOWN = 0,
    OWNERSHIP_NONE,
    OWNERSHIP_TAKEN
  };

  typedef base::Callback<void(OwnershipStatus)> OwnershipStatusCallback;
  typedef base::Callback<void(bool)> IsCurrentUserOwnerCallback;

  
  enum Status {
    STORE_SUCCESS,
    STORE_KEY_UNAVAILABLE,       
    STORE_POLICY_ERROR,          
    STORE_OPERATION_FAILED,      
    STORE_NO_POLICY,             
    STORE_INVALID_POLICY,        
    STORE_VALIDATION_ERROR,      
    STORE_TEMP_VALIDATION_ERROR, 
  };

  
  class Observer {
   public:
    virtual ~Observer();

    
    virtual void OwnershipStatusChanged() = 0;

    
    virtual void DeviceSettingsUpdated() = 0;
  };

  
  static void Initialize();
  static bool IsInitialized();
  static void Shutdown();
  static DeviceSettingsService* Get();

  
  
  DeviceSettingsService();
  virtual ~DeviceSettingsService();

  
  void SetSessionManager(SessionManagerClient* session_manager_client,
                         scoped_refptr<OwnerKeyUtil> owner_key_util);

  
  
  void UnsetSessionManager();

  
  
  const enterprise_management::PolicyData* policy_data() {
    return policy_data_.get();
  }
  const enterprise_management::ChromeDeviceSettingsProto*
      device_settings() const {
    return device_settings_.get();
  }

  
  scoped_refptr<OwnerKey> GetOwnerKey();

  
  Status status() {
    return store_status_;
  }

  
  
  void Load();

  
  
  
  
  
  void SignAndStore(
      scoped_ptr<enterprise_management::ChromeDeviceSettingsProto> new_settings,
      const base::Closure& callback);

  
  
  
  void Store(scoped_ptr<enterprise_management::PolicyFetchResponse> policy,
             const base::Closure& callback);

  
  
  OwnershipStatus GetOwnershipStatus();

  
  
  void GetOwnershipStatusAsync(const OwnershipStatusCallback& callback);

  
  bool HasPrivateOwnerKey();

  
  
  
  void IsCurrentUserOwnerAsync(const IsCurrentUserOwnerCallback& callback);

  
  
  void SetUsername(const std::string& username);
  const std::string& GetUsername() const;

  
  void AddObserver(Observer* observer);
  
  void RemoveObserver(Observer* observer);

  
  virtual void OwnerKeySet(bool success) OVERRIDE;
  virtual void PropertyChangeComplete(bool success) OVERRIDE;

  
  virtual void OnCertificatesLoaded(const net::CertificateList& cert_list,
                                    bool initial_load) OVERRIDE;

 private:
  
  
  void Enqueue(SessionManagerOperation* operation);

  
  void EnqueueLoad(bool force_key_load);

  
  
  void EnsureReload(bool force_key_load);

  
  void StartNextOperation();

  
  
  void HandleCompletedOperation(const base::Closure& callback,
                                SessionManagerOperation* operation,
                                Status status);

  SessionManagerClient* session_manager_client_;
  scoped_refptr<OwnerKeyUtil> owner_key_util_;

  base::WeakPtrFactory<DeviceSettingsService> weak_factory_;

  Status store_status_;

  std::vector<OwnershipStatusCallback> pending_ownership_status_callbacks_;
  std::vector<IsCurrentUserOwnerCallback>
      pending_is_current_user_owner_callbacks_;

  std::string username_;
  scoped_refptr<OwnerKey> owner_key_;
  
  bool certificates_loaded_;
  
  
  bool owner_key_loaded_with_certificates_;

  scoped_ptr<enterprise_management::PolicyData> policy_data_;
  scoped_ptr<enterprise_management::ChromeDeviceSettingsProto> device_settings_;

  
  
  std::deque<SessionManagerOperation*> pending_operations_;

  ObserverList<Observer, true> observers_;

  
  int load_retries_left_;

  DISALLOW_COPY_AND_ASSIGN(DeviceSettingsService);
};

class ScopedTestDeviceSettingsService {
 public:
  ScopedTestDeviceSettingsService();
  ~ScopedTestDeviceSettingsService();

 private:
  DISALLOW_COPY_AND_ASSIGN(ScopedTestDeviceSettingsService);
};

}  

#endif  
