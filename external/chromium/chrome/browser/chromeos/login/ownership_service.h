// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_OWNERSHIP_SERVICE_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_OWNERSHIP_SERVICE_H_
#pragma once

#include <string>
#include <vector>

#include "base/callback.h"
#include "base/synchronization/lock.h"
#include "chrome/browser/chromeos/login/owner_key_utils.h"
#include "chrome/browser/chromeos/login/owner_manager.h"
#include "chrome/browser/policy/proto/device_management_backend.pb.h"
#include "content/browser/browser_thread.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "content/common/notification_service.h"

namespace base {
template <typename T> struct DefaultLazyInstanceTraits;
}

namespace em = enterprise_management;
namespace chromeos {

class OwnershipService : public NotificationObserver {
 public:
  enum Status {
    
    OWNERSHIP_UNKNOWN = 0,
    OWNERSHIP_NONE,
    OWNERSHIP_TAKEN
  };

  
  static OwnershipService* GetSharedInstance();
  virtual ~OwnershipService();

  
  
  void Prewarm();

  
  
  
  
  
  
  
  virtual void set_cached_policy(const em::PolicyData& pol);
  virtual bool has_cached_policy();
  virtual const em::PolicyData& cached_policy();

  
  
  
  virtual void StartUpdateOwnerKey(const std::vector<uint8>& new_key,
                                   OwnerManager::KeyUpdateDelegate* d);

  
  
  
  
  
  virtual void StartLoadOwnerKeyAttempt();

  
  
  
  
  
  
  virtual void StartSigningAttempt(const std::string& data,
                                   OwnerManager::Delegate* d);

  
  
  
  
  
  
  virtual void StartVerifyAttempt(const std::string& data,
                                  const std::vector<uint8>& signature,
                                  OwnerManager::Delegate* d);

  
  virtual bool CurrentUserIsOwner();

  
  
  virtual bool IsAlreadyOwned();

  
  
  
  
  virtual Status GetStatus(bool blocking);

 protected:
  OwnershipService();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  friend struct base::DefaultLazyInstanceTraits<OwnershipService>;
  friend class OwnershipServiceTest;

  
  void FetchStatus();

  
  void SetStatus(Status new_status);

  static void UpdateOwnerKey(OwnershipService* service,
                             const BrowserThread::ID thread_id,
                             const std::vector<uint8>& new_key,
                             OwnerManager::KeyUpdateDelegate* d);
  static void TryLoadOwnerKeyAttempt(OwnershipService* service);
  static void TrySigningAttempt(OwnershipService* service,
                                const BrowserThread::ID thread_id,
                                const std::string& data,
                                OwnerManager::Delegate* d);
  static void TryVerifyAttempt(OwnershipService* service,
                               const BrowserThread::ID thread_id,
                               const std::string& data,
                               const std::vector<uint8>& signature,
                               OwnerManager::Delegate* d);
  static void FailAttempt(OwnerManager::Delegate* d);

  OwnerManager* manager() { return manager_.get(); }

  scoped_refptr<OwnerManager> manager_;
  scoped_refptr<OwnerKeyUtils> utils_;
  scoped_ptr<em::PolicyData> policy_;
  NotificationRegistrar notification_registrar_;
  volatile Status ownership_status_;
  base::Lock ownership_status_lock_;
};

}  

#endif  
