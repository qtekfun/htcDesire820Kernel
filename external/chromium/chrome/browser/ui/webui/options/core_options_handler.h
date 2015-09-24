// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_CORE_OPTIONS_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_CORE_OPTIONS_HANDLER_H_
#pragma once

#include <map>
#include <string>

#include "base/values.h"
#include "chrome/browser/plugin_data_remover_helper.h"
#include "chrome/browser/prefs/pref_change_registrar.h"
#include "chrome/browser/ui/webui/options/options_ui.h"

class CoreOptionsHandler : public OptionsPageUIHandler {
 public:
  CoreOptionsHandler();
  virtual ~CoreOptionsHandler();

  
  virtual void Initialize();
  virtual void GetLocalizedValues(DictionaryValue* localized_strings);
  virtual void Uninitialize();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  virtual void RegisterMessages();
  virtual WebUIMessageHandler* Attach(WebUI* web_ui);

  void set_handlers_host(OptionsPageUIHandlerHost* handlers_host) {
    handlers_host_ = handlers_host;
  }

 protected:
  
  
  virtual Value* FetchPref(const std::string& pref_name);

  
  virtual void ObservePref(const std::string& pref_name);

  
  virtual void SetPref(const std::string& pref_name,
                       const Value* value,
                       const std::string& metric);

  
  void ClearPref(const std::string& pref_name, const std::string& metric);

  
  virtual void StopObservingPref(const std::string& path);

  
  void ProcessUserMetric(const Value* value,
                         const std::string& metric);

  typedef std::multimap<std::string, std::wstring> PreferenceCallbackMap;
  PreferenceCallbackMap pref_callback_map_;
 private:
  
  
  
  void HandleInitialize(const ListValue* args);

  
  
  
  
  void HandleFetchPrefs(const ListValue* args);

  
  
  void HandleObservePrefs(const ListValue* args);

  
  
  
  
  
  void HandleSetBooleanPref(const ListValue* args);
  void HandleSetIntegerPref(const ListValue* args);
  void HandleSetDoublePref(const ListValue* args);
  void HandleSetStringPref(const ListValue* args);
  void HandleSetListPref(const ListValue* args);

  void HandleSetPref(const ListValue* args, Value::ValueType type);

  
  
  
  
  void HandleClearPref(const ListValue* args);

  
  
  
  void HandleUserMetricsAction(const ListValue* args);

  void UpdateClearPluginLSOData();

  void NotifyPrefChanged(const std::string* pref_name);

  OptionsPageUIHandlerHost* handlers_host_;
  PrefChangeRegistrar registrar_;

  
  
  PluginDataRemoverHelper clear_plugin_lso_data_enabled_;

  DISALLOW_COPY_AND_ASSIGN(CoreOptionsHandler);
};

#endif  
