// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SERVICES_GCM_GCM_PROFILE_SERVICE_H_
#define CHROME_BROWSER_SERVICES_GCM_GCM_PROFILE_SERVICE_H_

#include <map>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "google_apis/gcm/gcm_client.h"

class Profile;

namespace base {
class Value;
}

namespace extensions {
class Extension;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace gcm {

class GCMEventRouter;
class GCMProfileServiceTest;

class GCMProfileService : public BrowserContextKeyedService,
                          public content::NotificationObserver {
 public:
  typedef base::Callback<void(const std::string& registration_id,
                              GCMClient::Result result)> RegisterCallback;
  typedef base::Callback<void(const std::string& message_id,
                              GCMClient::Result result)> SendCallback;

  
  class TestingDelegate {
   public:
    virtual GCMEventRouter* GetEventRouter() const = 0;
    virtual void CheckInFinished(const GCMClient::CheckInInfo& checkin_info,
                                 GCMClient::Result result) = 0;
    virtual void LoadingFromPersistentStoreFinished() = 0;
  };

  
  static bool IsGCMEnabled();

  
  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  explicit GCMProfileService(Profile* profile);
  
  GCMProfileService(Profile* profile, TestingDelegate* testing_delegate);
  virtual ~GCMProfileService();

  
  
  
  
  
  
  
  
  virtual void Register(const std::string& app_id,
                        const std::vector<std::string>& sender_ids,
                        const std::string& cert,
                        RegisterCallback callback);

  
  
  
  
  
  virtual void Send(const std::string& app_id,
                    const std::string& receiver_id,
                    const GCMClient::OutgoingMessage& message,
                    SendCallback callback);

 protected:
  
  
  static bool enable_gcm_for_testing_;

 private:
  friend class GCMProfileServiceTest;
  friend class GCMProfileServiceRegisterTest;
  FRIEND_TEST_ALL_PREFIXES(GCMProfileServiceTest, CheckInFromPrefsStore);
  FRIEND_TEST_ALL_PREFIXES(GCMProfileServiceTest, CheckOut);
  FRIEND_TEST_ALL_PREFIXES(GCMProfileServiceRegisterTest, Unregister);

  class IOWorker;

  struct RegistrationInfo {
    RegistrationInfo();
    ~RegistrationInfo();

    std::vector<std::string> sender_ids;
    std::string registration_id;
  };

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  void Init();

  
  
  
  void AddUser();

  
  
  void RemoveUser();

  
  void Unregister(const std::string& app_id);

  void CheckInFinished(GCMClient::CheckInInfo checkin_info,
                       GCMClient::Result result);
  void RegisterFinished(std::string app_id,
                        std::string registration_id,
                        GCMClient::Result result);
  void SendFinished(std::string app_id,
                    std::string message_id,
                    GCMClient::Result result);
  void MessageReceived(std::string app_id,
                       GCMClient::IncomingMessage message);
  void MessagesDeleted(std::string app_id);
  void MessageSendError(std::string app_id,
                        std::string message_id,
                        GCMClient::Result result);

  
  GCMEventRouter* GetEventRouter(const std::string& app_id);

  
  void DeleteRegistrationInfo(const std::string& app_id);
  void WriteRegistrationInfo(const std::string& app_id);
  void ReadRegistrationInfo(const std::string& app_id);
  void ReadRegistrationInfoFinished(std::string app_id,
                                    scoped_ptr<base::Value> value);
  bool ParsePersistedRegistrationInfo(scoped_ptr<base::Value> value,
                                      RegistrationInfo* registration_info);

  
  
  static const char* GetPersistentRegisterKeyForTesting();

  
  Profile* profile_;

  
  std::string username_;

  content::NotificationRegistrar registrar_;

  
  scoped_refptr<IOWorker> io_worker_;

  
  std::map<std::string, RegisterCallback> register_callbacks_;

  
  std::map<std::pair<std::string, std::string>, SendCallback> send_callbacks_;

  
  typedef std::map<std::string, RegistrationInfo> RegistrationInfoMap;
  RegistrationInfoMap registration_info_map_;

  
  scoped_ptr<GCMEventRouter> js_event_router_;

  
  TestingDelegate* testing_delegate_;

  
  base::WeakPtrFactory<GCMProfileService> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(GCMProfileService);
};

}  

#endif  
