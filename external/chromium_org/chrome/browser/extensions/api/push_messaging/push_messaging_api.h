// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_PUSH_MESSAGING_PUSH_MESSAGING_API_H__
#define CHROME_BROWSER_EXTENSIONS_API_PUSH_MESSAGING_PUSH_MESSAGING_API_H__

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/extensions/api/profile_keyed_api_factory.h"
#include "chrome/browser/extensions/api/push_messaging/obfuscated_gaia_id_fetcher.h"
#include "chrome/browser/extensions/api/push_messaging/push_messaging_invalidation_handler_delegate.h"
#include "chrome/browser/extensions/chrome_extension_function.h"
#include "chrome/browser/ui/webui/signin/login_ui_service.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "google_apis/gaia/google_service_auth_error.h"
#include "google_apis/gaia/oauth2_token_service.h"

class Profile;

namespace extensions {

class PushMessagingInvalidationMapper;
class ObfuscatedGaiaIdFetcher;

class PushMessagingEventRouter
    : public PushMessagingInvalidationHandlerDelegate {
 public:
  explicit PushMessagingEventRouter(Profile* profile);
  virtual ~PushMessagingEventRouter();

  
  void TriggerMessageForTest(const std::string& extension_id,
                             int subchannel,
                             const std::string& payload);

 private:
  
  virtual void OnMessage(const std::string& extension_id,
                         int subchannel,
                         const std::string& payload) OVERRIDE;

  Profile* const profile_;

  DISALLOW_COPY_AND_ASSIGN(PushMessagingEventRouter);
};

class PushMessagingGetChannelIdFunction
    : public ChromeAsyncExtensionFunction,
      public ObfuscatedGaiaIdFetcher::Delegate,
      public OAuth2TokenService::Observer,
      public OAuth2TokenService::Consumer {
 public:
  PushMessagingGetChannelIdFunction();

 protected:
  virtual ~PushMessagingGetChannelIdFunction();

  
  virtual bool RunImpl() OVERRIDE;
  DECLARE_EXTENSION_FUNCTION("pushMessaging.getChannelId",
                             PUSHMESSAGING_GETCHANNELID)

 private:
  void ReportResult(const std::string& gaia_id,
                    const std::string& error_message);

  void BuildAndSendResult(const std::string& gaia_id,
                          const std::string& error_message);

  
  void StartGaiaIdFetch(const std::string& access_token);

  
  void StartAccessTokenFetch();

  
  virtual void OnRefreshTokenAvailable(const std::string& account_id) OVERRIDE;

  
  virtual void OnGetTokenSuccess(
      const OAuth2TokenService::Request* request,
      const std::string& access_token,
      const base::Time& expiration_time) OVERRIDE;
  virtual void OnGetTokenFailure(
      const OAuth2TokenService::Request* request,
      const GoogleServiceAuthError& error) OVERRIDE;

  
  bool IsUserLoggedIn() const;

  
  virtual void OnObfuscatedGaiaIdFetchSuccess(const std::string& gaia_id)
      OVERRIDE;
  virtual void OnObfuscatedGaiaIdFetchFailure(
      const GoogleServiceAuthError& error) OVERRIDE;
  scoped_ptr<ObfuscatedGaiaIdFetcher> fetcher_;
  bool interactive_;
  scoped_ptr<OAuth2TokenService::Request> fetcher_access_token_request_;

  DISALLOW_COPY_AND_ASSIGN(PushMessagingGetChannelIdFunction);
};

class PushMessagingAPI : public ProfileKeyedAPI,
                         public content::NotificationObserver {
 public:
  explicit PushMessagingAPI(Profile* profile);
  virtual ~PushMessagingAPI();

  
  static PushMessagingAPI* Get(Profile* profile);

  
  virtual void Shutdown() OVERRIDE;

  
  static ProfileKeyedAPIFactory<PushMessagingAPI>* GetFactoryInstance();

  
  PushMessagingEventRouter* GetEventRouterForTest() const {
  return event_router_.get();
  }
  PushMessagingInvalidationMapper* GetMapperForTest() const {
    return handler_.get();
  }
  void SetMapperForTest(scoped_ptr<PushMessagingInvalidationMapper> mapper);

 private:
  friend class ProfileKeyedAPIFactory<PushMessagingAPI>;

  
  static const char* service_name() {
    return "PushMessagingAPI";
  }
  static const bool kServiceIsNULLWhileTesting = true;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  scoped_ptr<PushMessagingEventRouter> event_router_;
  scoped_ptr<PushMessagingInvalidationMapper> handler_;

  content::NotificationRegistrar registrar_;

  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(PushMessagingAPI);
};

template <>
void ProfileKeyedAPIFactory<PushMessagingAPI>::DeclareFactoryDependencies();

}  

#endif  
