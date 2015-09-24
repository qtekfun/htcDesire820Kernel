// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_SESSION_MANAGER_CLIENT_H_
#define CHROMEOS_DBUS_SESSION_MANAGER_CLIENT_H_

#include <map>
#include <string>

#include "base/callback.h"
#include "base/observer_list.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/dbus_client.h"
#include "chromeos/dbus/dbus_client_implementation_type.h"

namespace chromeos {

class CHROMEOS_EXPORT SessionManagerClient : public DBusClient {
 public:
  
  class Observer {
   public:
    
    virtual void OwnerKeySet(bool success) {}

    
    virtual void PropertyChangeComplete(bool success) {}

    
    
    
    
    virtual void LockScreen() {}

    
    
    virtual void ScreenIsLocked() {}

    
    
    
    virtual void ScreenIsUnlocked() {}

    
    virtual void EmitLoginPromptVisibleCalled() {}
  };

  
  virtual void AddObserver(Observer* observer) = 0;
  virtual void RemoveObserver(Observer* observer) = 0;
  virtual bool HasObserver(Observer* observer) = 0;

  
  virtual void EmitLoginPromptReady() = 0;

  
  virtual void EmitLoginPromptVisible() = 0;

  
  virtual void RestartJob(int pid, const std::string& command_line) = 0;

  
  virtual void StartSession(const std::string& user_email) = 0;

  
  virtual void StopSession() = 0;

  
  virtual void StartDeviceWipe() = 0;

  
  virtual void RequestLockScreen() = 0;

  
  virtual void NotifyLockScreenShown() = 0;

  
  virtual void NotifyLockScreenDismissed() = 0;

  
  
  typedef std::map<std::string, std::string> ActiveSessionsMap;

  
  
  
  
  typedef base::Callback<void(const ActiveSessionsMap& sessions,
                              bool success)> ActiveSessionsCallback;

  
  
  
  
  
  virtual void RetrieveActiveSessions(
      const ActiveSessionsCallback& callback) = 0;

  
  
  
  
  typedef base::Callback<void(const std::string&)> RetrievePolicyCallback;

  
  
  virtual void RetrieveDevicePolicy(const RetrievePolicyCallback& callback) = 0;

  
  
  
  virtual void RetrievePolicyForUser(
      const std::string& username,
      const RetrievePolicyCallback& callback) = 0;

  
  
  
  
  
  
  virtual std::string BlockingRetrievePolicyForUser(
      const std::string& username) = 0;

  
  
  virtual void RetrieveDeviceLocalAccountPolicy(
      const std::string& account_id,
      const RetrievePolicyCallback& callback) = 0;

  
  
  
  typedef base::Callback<void(bool)> StorePolicyCallback;

  
  
  virtual void StoreDevicePolicy(const std::string& policy_blob,
                                 const StorePolicyCallback& callback) = 0;

  
  
  
  
  virtual void StorePolicyForUser(const std::string& username,
                                  const std::string& policy_blob,
                                  const std::string& policy_key,
                                  const StorePolicyCallback& callback) = 0;

  
  
  virtual void StoreDeviceLocalAccountPolicy(
      const std::string& account_id,
      const std::string& policy_blob,
      const StorePolicyCallback& callback) = 0;

  
  
  virtual void SetFlagsForUser(const std::string& username,
                               const std::vector<std::string>& flags) = 0;

  
  static SessionManagerClient* Create(DBusClientImplementationType type);

  virtual ~SessionManagerClient();

 protected:
  
  SessionManagerClient();

 private:
  DISALLOW_COPY_AND_ASSIGN(SessionManagerClient);
};

}  

#endif  
