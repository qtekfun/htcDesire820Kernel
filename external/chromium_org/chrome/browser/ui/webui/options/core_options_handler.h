// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_CORE_OPTIONS_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_CORE_OPTIONS_HANDLER_H_

#include <map>
#include <string>

#include "base/callback.h"
#include "base/prefs/pref_change_registrar.h"
#include "base/prefs/pref_service.h"
#include "base/values.h"
#include "chrome/browser/plugins/plugin_status_pref_setter.h"
#include "chrome/browser/ui/webui/options/options_ui.h"

namespace options {

class CoreOptionsHandler : public OptionsPageUIHandler {
 public:
  CoreOptionsHandler();
  virtual ~CoreOptionsHandler();

  
  virtual void GetLocalizedValues(DictionaryValue* localized_strings) OVERRIDE;
  virtual void InitializeHandler() OVERRIDE;
  virtual void InitializePage() OVERRIDE;
  virtual void Uninitialize() OVERRIDE;

  
  virtual void RegisterMessages() OVERRIDE;

  void set_handlers_host(OptionsPageUIHandlerHost* handlers_host) {
    handlers_host_ = handlers_host;
  }

  
  static void GetStaticLocalizedValues(
      base::DictionaryValue* localized_strings);

 protected:
  
  
  virtual base::Value* FetchPref(const std::string& pref_name);

  
  virtual void ObservePref(const std::string& pref_name);

  
  virtual void StopObservingPref(const std::string& pref_name);

  
  virtual void SetPref(const std::string& pref_name,
                       const base::Value* value,
                       const std::string& metric);

  
  void ClearPref(const std::string& pref_name, const std::string& metric);

  
  void ProcessUserMetric(const base::Value* value,
                         const std::string& metric);

  
  
  
  
  virtual void OnPreferenceChanged(PrefService* service,
                                   const std::string& pref_name);

  
  
  
  
  void NotifyPrefChanged(const std::string& pref_name,
                         const std::string& controlling_pref_name);

  
  
  
  void DispatchPrefChangeNotification(const std::string& name,
                                      scoped_ptr<base::Value> value);

  
  
  
  base::Value* CreateValueForPref(const std::string& pref_name,
                                  const std::string& controlling_pref_name);

  typedef std::multimap<std::string, std::string> PreferenceCallbackMap;
  PreferenceCallbackMap pref_callback_map_;

 private:
  
  
  enum PrefType {
    TYPE_BOOLEAN = 0,
    TYPE_INTEGER,
    TYPE_DOUBLE,
    TYPE_STRING,
    TYPE_URL,
    TYPE_LIST,
  };

  
  
  PrefService* FindServiceForPref(const std::string& pref_name);

  
  
  
  void HandleInitialize(const ListValue* args);

  
  
  
  
  void HandleFetchPrefs(const ListValue* args);

  
  
  void HandleObservePrefs(const ListValue* args);

  
  
  
  
  
  void HandleSetBooleanPref(const ListValue* args);
  void HandleSetIntegerPref(const ListValue* args);
  void HandleSetDoublePref(const ListValue* args);
  void HandleSetStringPref(const ListValue* args);
  void HandleSetURLPref(const ListValue* args);
  void HandleSetListPref(const ListValue* args);

  void HandleSetPref(const ListValue* args, PrefType type);

  
  
  
  
  void HandleClearPref(const ListValue* args);

  
  
  
  void HandleUserMetricsAction(const ListValue* args);

  
  
  void HandleDisableExtension(const ListValue* args);

  void UpdateClearPluginLSOData();
  void UpdatePepperFlashSettingsEnabled();

  OptionsPageUIHandlerHost* handlers_host_;
  
  PrefChangeRegistrar registrar_;
  
  PrefChangeRegistrar local_state_registrar_;

  PluginStatusPrefSetter plugin_status_pref_setter_;

  
  
  
  
  typedef std::map<std::string, base::Callback<bool(const base::Value*)> >
      PrefChangeFilterMap;
  PrefChangeFilterMap pref_change_filters_;

  DISALLOW_COPY_AND_ASSIGN(CoreOptionsHandler);
};

}  

#endif  
