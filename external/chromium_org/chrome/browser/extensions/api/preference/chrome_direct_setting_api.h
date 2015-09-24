// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_PREFERENCE_CHROME_DIRECT_SETTING_API_H__
#define CHROME_BROWSER_EXTENSIONS_API_PREFERENCE_CHROME_DIRECT_SETTING_API_H__

#include "base/prefs/pref_change_registrar.h"
#include "chrome/browser/extensions/api/profile_keyed_api_factory.h"
#include "extensions/browser/event_router.h"

class Profile;

namespace extensions {
namespace chromedirectsetting {

class ChromeDirectSettingAPI : public ProfileKeyedAPI,
                               public EventRouter::Observer {
 public:
  explicit ChromeDirectSettingAPI(Profile* profile);

  virtual ~ChromeDirectSettingAPI();

  
  virtual void Shutdown() OVERRIDE;

  
  static ProfileKeyedAPIFactory<ChromeDirectSettingAPI>* GetFactoryInstance();

  
  virtual void OnListenerAdded(const EventListenerInfo& details) OVERRIDE;

  
  bool IsPreferenceOnWhitelist(const std::string& pref_key);

  
  static ChromeDirectSettingAPI* Get(Profile* profile);

 private:
  friend class ProfileKeyedAPIFactory<ChromeDirectSettingAPI>;

  
  static const char* service_name();

  void OnPrefChanged(PrefService* pref_service, const std::string& pref_key);

  static const bool kServiceIsNULLWhileTesting = true;
  static const bool kServiceRedirectedInIncognito = false;

  PrefChangeRegistrar registrar_;
  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(ChromeDirectSettingAPI);
};

}  
}  

#endif  

