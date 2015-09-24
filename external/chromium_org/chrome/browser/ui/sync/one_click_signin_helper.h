// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_SYNC_ONE_CLICK_SIGNIN_HELPER_H_
#define CHROME_BROWSER_UI_SYNC_ONE_CLICK_SIGNIN_HELPER_H_

#include <string>

#include "base/gtest_prod_util.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/signin/signin_promo.h"
#include "chrome/browser/sync/profile_sync_service_observer.h"
#include "chrome/browser/ui/sync/one_click_signin_sync_starter.h"
#include "content/public/browser/navigation_controller.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"
#include "google_apis/gaia/google_service_auth_error.h"

class Browser;
class GURL;
class PasswordManager;
class ProfileIOData;

namespace autofill {
struct PasswordForm;
}

namespace content {
class WebContents;
struct FrameNavigateParams;
struct LoadCommittedDetails;
}

namespace net {
class URLRequest;
}

class OneClickSigninHelper
    : public content::WebContentsObserver,
      public content::WebContentsUserData<OneClickSigninHelper>,
      public ProfileSyncServiceObserver {
 public:
  
  enum AutoAccept {
    
    AUTO_ACCEPT_NONE,

    
    
    AUTO_ACCEPT_ACCEPTED,

    
    
    AUTO_ACCEPT_CONFIGURE,

    
    
    AUTO_ACCEPT_REJECTED_FOR_PROFILE,

    
    
    
    AUTO_ACCEPT_EXPLICIT
  };

  
  enum Offer {
    CAN_OFFER,
    DONT_OFFER,
    IGNORE_REQUEST
  };

  
  enum CanOfferFor {
    CAN_OFFER_FOR_ALL,
    CAN_OFFER_FOR_INTERSTITAL_ONLY,
    CAN_OFFER_FOR_SECONDARY_ACCOUNT
    
    
  };

  static void CreateForWebContentsWithPasswordManager(
      content::WebContents* contents,
      PasswordManager* password_manager);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static bool CanOffer(content::WebContents* web_contents,
                       CanOfferFor can_offer_for,
                       const std::string& email,
                       std::string* error_message);

  
  
  
  
  static Offer CanOfferOnIOThread(net::URLRequest* request,
                                  ProfileIOData* io_data);

  
  
  
  static void ShowInfoBarIfPossible(net::URLRequest* request,
                                    ProfileIOData* io_data,
                                    int child_id,
                                    int route_id);

  
  
  static void RedirectToNtpOrAppsPageIfNecessary(
      content::WebContents* contents, signin::Source source);

  static void ShowSigninErrorBubble(Browser* browser, const std::string& error);

  
  
  
  static void RemoveSigninRedirectURLHistoryItem(
      content::WebContents* web_contents);

  static void LogConfirmHistogramValue(int action);

 private:
  friend class content::WebContentsUserData<OneClickSigninHelper>;
  friend class OneClickSigninHelperTest;
  FRIEND_TEST_ALL_PREFIXES(OneClickSigninHelperIncognitoTest,
                           ShowInfoBarUIThreadIncognito);
  FRIEND_TEST_ALL_PREFIXES(OneClickSigninHelperTest,
                           SigninFromWebstoreWithConfigSyncfirst);
  FRIEND_TEST_ALL_PREFIXES(OneClickSigninHelperTest,
                           ShowSigninBubbleAfterSigninComplete);
  FRIEND_TEST_ALL_PREFIXES(OneClickSigninHelperTest, SigninCancelled);
  FRIEND_TEST_ALL_PREFIXES(OneClickSigninHelperTest, SigninFailed);
  FRIEND_TEST_ALL_PREFIXES(OneClickSigninHelperTest,
                           CleanTransientStateOnNavigate);
  FRIEND_TEST_ALL_PREFIXES(OneClickSigninHelperTest,
                           RemoveObserverFromProfileSyncService);
  FRIEND_TEST_ALL_PREFIXES(OneClickSigninHelperIOTest, CanOfferOnIOThread);
  FRIEND_TEST_ALL_PREFIXES(OneClickSigninHelperIOTest,
                           CanOfferOnIOThreadIncognito);
  FRIEND_TEST_ALL_PREFIXES(OneClickSigninHelperIOTest,
                           CanOfferOnIOThreadNoIOData);
  FRIEND_TEST_ALL_PREFIXES(OneClickSigninHelperIOTest,
                           CanOfferOnIOThreadBadURL);
  FRIEND_TEST_ALL_PREFIXES(OneClickSigninHelperIOTest,
                           CanOfferOnIOThreadReferrer);
  FRIEND_TEST_ALL_PREFIXES(OneClickSigninHelperIOTest,
                           CanOfferOnIOThreadDisabled);
  FRIEND_TEST_ALL_PREFIXES(OneClickSigninHelperIOTest,
                           CanOfferOnIOThreadSignedIn);
  FRIEND_TEST_ALL_PREFIXES(OneClickSigninHelperIOTest,
                           CanOfferOnIOThreadEmailNotAllowed);
  FRIEND_TEST_ALL_PREFIXES(OneClickSigninHelperIOTest,
                           CanOfferOnIOThreadEmailAlreadyUsed);
  FRIEND_TEST_ALL_PREFIXES(OneClickSigninHelperIOTest,
                           CreateTestProfileIOData);
  FRIEND_TEST_ALL_PREFIXES(OneClickSigninHelperIOTest,
                           CanOfferOnIOThreadWithRejectedEmail);
  FRIEND_TEST_ALL_PREFIXES(OneClickSigninHelperIOTest,
                           CanOfferOnIOThreadNoSigninCookies);
  FRIEND_TEST_ALL_PREFIXES(OneClickSigninHelperIOTest,
                           CanOfferOnIOThreadDisabledByPolicy);

  
  
  
  static const int kMaxNavigationsSince;

  OneClickSigninHelper(content::WebContents* web_contents,
                       PasswordManager* password_manager);

  virtual ~OneClickSigninHelper();

  
  
  
  
  static Offer CanOfferOnIOThreadImpl(const GURL& url,
                                      const std::string& referrer,
                                      base::SupportsUserData* request,
                                      ProfileIOData* io_data);

  
  
  
  
  
  
  
  
  
  
  
  static void ShowInfoBarUIThread(const std::string& session_index,
                                  const std::string& email,
                                  AutoAccept auto_accept,
                                  signin::Source source,
                                  const GURL& continue_url,
                                  int child_id,
                                  int route_id);

  void RedirectToSignin();

  
  void CleanTransientState();

  
  
  
  
  void SetDoNotClearPendingEmailForTesting();

  
  void set_do_not_start_sync_for_testing();

  
  void PasswordSubmitted(const autofill::PasswordForm& form);

  
  virtual void DidStartNavigationToPendingEntry(
      const GURL& url,
      content::NavigationController::ReloadType reload_type) OVERRIDE;
  virtual void DidNavigateMainFrame(
      const content::LoadCommittedDetails& details,
      const content::FrameNavigateParams& params) OVERRIDE;
  virtual void DidStopLoading(
      content::RenderViewHost* render_view_host) OVERRIDE;
  virtual void WebContentsDestroyed(content::WebContents* contents) OVERRIDE;

  
  virtual void OnStateChanged() OVERRIDE;

  OneClickSigninSyncStarter::Callback CreateSyncStarterCallback();

  
  void SyncSetupCompletedCallback(
      OneClickSigninSyncStarter::SyncSetupResult result);

  
  
  
  
  bool showing_signin_;

  
  std::string session_index_;
  std::string email_;
  std::string password_;
  AutoAccept auto_accept_;
  signin::Source source_;
  bool switched_to_advanced_;
  GURL continue_url_;
  
  GURL original_continue_url_;
  std::string error_message_;

  
  
  
  
  int untrusted_navigations_since_signin_visit_;

  
  
  
  
  bool untrusted_confirmation_required_;

  
  
  bool do_not_clear_pending_email_;

  
  bool do_not_start_sync_for_testing_;

  base::WeakPtrFactory<OneClickSigninHelper> weak_pointer_factory_;

  DISALLOW_COPY_AND_ASSIGN(OneClickSigninHelper);
};

#endif  
