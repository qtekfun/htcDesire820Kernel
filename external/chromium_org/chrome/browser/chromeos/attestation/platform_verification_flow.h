// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_ATTESTATION_PLATFORM_VERIFICATION_FLOW_H_
#define CHROME_BROWSER_CHROMEOS_ATTESTATION_PLATFORM_VERIFICATION_FLOW_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "url/gurl.h"

class HostContentSettingsMap;
class PrefService;

namespace content {
class WebContents;
}

namespace cryptohome {
class AsyncMethodCaller;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace chromeos {

class CryptohomeClient;
class UserManager;
class User;

namespace attestation {

class AttestationFlow;
class PlatformVerificationFlowTest;

class PlatformVerificationFlow
    : public base::RefCountedThreadSafe<PlatformVerificationFlow> {
 public:
  enum Result {
    SUCCESS,                
    INTERNAL_ERROR,         
    PLATFORM_NOT_VERIFIED,  
                            
                            
    USER_REJECTED,          
    POLICY_REJECTED,        
    TIMEOUT,                
  };

  enum ConsentResponse {
    CONSENT_RESPONSE_NONE,
    CONSENT_RESPONSE_ALLOW,
    CONSENT_RESPONSE_DENY,
  };

  
  
  class Delegate {
   public:
    virtual ~Delegate() {}

    
    
    typedef base::Callback<void(ConsentResponse response)> ConsentCallback;

    
    
    
    
    virtual void ShowConsentPrompt(content::WebContents* web_contents,
                                   const ConsentCallback& callback) = 0;
  };

  
  
  
  
  
  
  
  
  typedef base::Callback<void(Result result,
                              const std::string& signed_data,
                              const std::string& signature,
                              const std::string& platform_key_certificate)>
      ChallengeCallback;

  
  
  PlatformVerificationFlow();

  
  
  PlatformVerificationFlow(AttestationFlow* attestation_flow,
                           cryptohome::AsyncMethodCaller* async_caller,
                           CryptohomeClient* cryptohome_client,
                           UserManager* user_manager,
                           Delegate* delegate);

  
  
  
  
  
  
  
  
  
  
  void ChallengePlatformKey(content::WebContents* web_contents,
                            const std::string& service_id,
                            const std::string& challenge,
                            const ChallengeCallback& callback);

  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* prefs);

  void set_timeout_delay(const base::TimeDelta& timeout_delay) {
    timeout_delay_ = timeout_delay;
  }

 private:
  friend class base::RefCountedThreadSafe<PlatformVerificationFlow>;
  friend class PlatformVerificationFlowTest;

  
  
  struct ChallengeContext {
    ChallengeContext(content::WebContents* web_contents,
                     const std::string& service_id,
                     const std::string& challenge,
                     const ChallengeCallback& callback);
    ~ChallengeContext();

    content::WebContents* web_contents;
    std::string service_id;
    std::string challenge;
    ChallengeCallback callback;
  };

  ~PlatformVerificationFlow();

  
  
  
  
  void CheckConsent(const ChallengeContext& context,
                    bool attestation_enrolled);

  
  
  
  
  
  void OnConsentResponse(const ChallengeContext& context,
                         bool consent_required,
                         ConsentResponse consent_response);

  
  
  
  
  
  void GetCertificate(const ChallengeContext& context,
                      const std::string& user_id,
                      bool force_new_key);

  
  
  
  
  
  
  
  
  
  void OnCertificateReady(const ChallengeContext& context,
                          const std::string& user_id,
                          scoped_ptr<base::Timer> timer,
                          bool operation_success,
                          const std::string& certificate);

  
  
  
  void OnCertificateTimeout(const ChallengeContext& context);

  
  
  
  
  
  
  void OnChallengeReady(const ChallengeContext& context,
                        const std::string& certificate,
                        bool operation_success,
                        const std::string& response_data);

  
  
  
  PrefService* GetPrefs(content::WebContents* web_contents);

  
  
  const GURL& GetURL(content::WebContents* web_contents);

  
  
  
  User* GetUser(content::WebContents* web_contents);

  
  
  HostContentSettingsMap* GetContentSettings(
      content::WebContents* web_contents);

  
  
  bool IsAttestationEnabled(content::WebContents* web_contents);

  
  
  bool UpdateSettings(content::WebContents* web_contents,
                      ConsentResponse consent_response);

  
  
  
  bool GetDomainPref(HostContentSettingsMap* content_settings,
                     const GURL& url,
                     bool* pref_value);

  
  
  void RecordDomainConsent(HostContentSettingsMap* content_settings,
                           const GURL& url,
                           bool allow_domain);

  
  bool IsExpired(const std::string& certificate);

  
  bool IsGuestOrIncognito(content::WebContents* web_contents);

  void set_testing_prefs(PrefService* testing_prefs) {
    testing_prefs_ = testing_prefs;
  }

  void set_testing_url(const GURL& testing_url) {
    testing_url_ = testing_url;
  }

  void set_testing_content_settings(HostContentSettingsMap* settings) {
    testing_content_settings_ = settings;
  }

  AttestationFlow* attestation_flow_;
  scoped_ptr<AttestationFlow> default_attestation_flow_;
  cryptohome::AsyncMethodCaller* async_caller_;
  CryptohomeClient* cryptohome_client_;
  UserManager* user_manager_;
  Delegate* delegate_;
  scoped_ptr<Delegate> default_delegate_;
  PrefService* testing_prefs_;
  GURL testing_url_;
  HostContentSettingsMap* testing_content_settings_;
  base::TimeDelta timeout_delay_;

  DISALLOW_COPY_AND_ASSIGN(PlatformVerificationFlow);
};

}  
}  

#endif  
