// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_PREFERENCE_PREFERENCE_API_H__
#define CHROME_BROWSER_EXTENSIONS_API_PREFERENCE_PREFERENCE_API_H__

#include <string>

#include "base/prefs/pref_change_registrar.h"
#include "chrome/browser/extensions/api/content_settings/content_settings_store.h"
#include "chrome/browser/extensions/api/profile_keyed_api_factory.h"
#include "chrome/browser/extensions/chrome_extension_function.h"
#include "content/public/browser/notification_observer.h"
#include "extensions/browser/event_router.h"
#include "extensions/browser/extension_prefs_scope.h"

class ExtensionPrefValueMap;
class PrefService;

namespace base {
class Value;
}

namespace extensions {
class ExtensionPrefs;

class PreferenceEventRouter {
 public:
  explicit PreferenceEventRouter(Profile* profile);
  virtual ~PreferenceEventRouter();

 private:
  void OnPrefChanged(PrefService* pref_service,
                     const std::string& pref_key);

  PrefChangeRegistrar registrar_;
  PrefChangeRegistrar incognito_registrar_;

  
  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(PreferenceEventRouter);
};

class PreferenceAPIBase {
 public:
  
  
  

  
  
  void SetExtensionControlledPref(const std::string& extension_id,
                                  const std::string& pref_key,
                                  ExtensionPrefsScope scope,
                                  base::Value* value);

  
  void RemoveExtensionControlledPref(const std::string& extension_id,
                                     const std::string& pref_key,
                                     ExtensionPrefsScope scope);

  
  
  bool CanExtensionControlPref(const std::string& extension_id,
                               const std::string& pref_key,
                               bool incognito);

  
  
  
  
  
  bool DoesExtensionControlPref(const std::string& extension_id,
                                const std::string& pref_key,
                                bool* from_incognito);

 protected:
  
  virtual ExtensionPrefs* extension_prefs() = 0;
  virtual ExtensionPrefValueMap* extension_pref_value_map() = 0;
};

class PreferenceAPI : public PreferenceAPIBase,
                      public ProfileKeyedAPI,
                      public EventRouter::Observer,
                      public ContentSettingsStore::Observer {
 public:
  explicit PreferenceAPI(Profile* profile);
  virtual ~PreferenceAPI();

  
  virtual void Shutdown() OVERRIDE;

  
  static ProfileKeyedAPIFactory<PreferenceAPI>* GetFactoryInstance();

  
  static PreferenceAPI* Get(Profile* profile);

  
  virtual void OnListenerAdded(const EventListenerInfo& details) OVERRIDE;

  
  
  static void LoadExtensionControlledPrefs(ExtensionPrefs* prefs,
                                           ExtensionPrefValueMap* value_map,
                                           const std::string& extension_id,
                                           ExtensionPrefsScope scope);

  
  
  
  static void InitExtensionControlledPrefs(ExtensionPrefs* prefs,
                                           ExtensionPrefValueMap* value_map);

 private:
  friend class ProfileKeyedAPIFactory<PreferenceAPI>;

  
  virtual void OnContentSettingChanged(const std::string& extension_id,
                                       bool incognito) OVERRIDE;

  
  void ClearIncognitoSessionOnlyContentSettings();

  
  virtual ExtensionPrefs* extension_prefs() OVERRIDE;
  virtual ExtensionPrefValueMap* extension_pref_value_map() OVERRIDE;

  Profile* profile_;

  
  static const char* service_name() {
    return "PreferenceAPI";
  }
  static const bool kServiceIsNULLWhileTesting = true;
  static const bool kServiceRedirectedInIncognito = true;

  
  scoped_ptr<PreferenceEventRouter> preference_event_router_;

  DISALLOW_COPY_AND_ASSIGN(PreferenceAPI);
};

class PrefTransformerInterface {
 public:
  virtual ~PrefTransformerInterface() {}

  
  
  
  
  
  
  virtual base::Value* ExtensionToBrowserPref(
      const base::Value* extension_pref,
      std::string* error,
      bool* bad_message) = 0;

  
  
  
  
  virtual base::Value* BrowserToExtensionPref(
      const base::Value* browser_pref) = 0;
};

class PreferenceFunction : public ChromeSyncExtensionFunction {
 protected:
  virtual ~PreferenceFunction();

  
  
  
  
  
  bool ValidateBrowserPref(const std::string& extension_pref_key,
                           std::string* browser_pref_key);
};

class GetPreferenceFunction : public PreferenceFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("types.ChromeSetting.get", TYPES_CHROMESETTING_GET)

 protected:
  virtual ~GetPreferenceFunction();

  
  virtual bool RunImpl() OVERRIDE;
};

class SetPreferenceFunction : public PreferenceFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("types.ChromeSetting.set", TYPES_CHROMESETTING_SET)

 protected:
  virtual ~SetPreferenceFunction();

  
  virtual bool RunImpl() OVERRIDE;
};

class ClearPreferenceFunction : public PreferenceFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("types.ChromeSetting.clear",
                             TYPES_CHROMESETTING_CLEAR)

 protected:
  virtual ~ClearPreferenceFunction();

  
  virtual bool RunImpl() OVERRIDE;
};

}  

#endif  
