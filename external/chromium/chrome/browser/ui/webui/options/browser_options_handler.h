// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_BROWSER_OPTIONS_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_BROWSER_OPTIONS_HANDLER_H_
#pragma once

#include "chrome/browser/autocomplete/autocomplete_controller_delegate.h"
#include "chrome/browser/prefs/pref_member.h"
#include "chrome/browser/search_engines/template_url_model_observer.h"
#include "chrome/browser/shell_integration.h"
#include "chrome/browser/ui/webui/options/options_ui.h"
#include "ui/base/models/table_model_observer.h"

class AutocompleteController;
class CustomHomePagesTableModel;
class OptionsManagedBannerHandler;
class TemplateURLModel;

class BrowserOptionsHandler : public OptionsPageUIHandler,
                              public AutocompleteControllerDelegate,
                              public ShellIntegration::DefaultBrowserObserver,
                              public TemplateURLModelObserver,
                              public ui::TableModelObserver {
 public:
  BrowserOptionsHandler();
  virtual ~BrowserOptionsHandler();

  virtual void Initialize();

  
  virtual void GetLocalizedValues(DictionaryValue* localized_strings);
  virtual void RegisterMessages();

  
  virtual void OnResultChanged(bool default_match_changed);

  
  virtual void SetDefaultBrowserUIState(
      ShellIntegration::DefaultBrowserUIState state);

  
  virtual void OnTemplateURLModelChanged();

  
  virtual void OnModelChanged();
  virtual void OnItemsChanged(int start, int length);
  virtual void OnItemsAdded(int start, int length);
  virtual void OnItemsRemoved(int start, int length);

 private:
  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  void SetHomePage(const ListValue* args);

  
  void BecomeDefaultBrowser(const ListValue* args);

  
  void SetDefaultSearchEngine(const ListValue* args);

  
  void RemoveStartupPages(const ListValue* args);

  
  
  void AddStartupPage(const ListValue* args);

  
  
  void EditStartupPage(const ListValue* args);

  
  void SetStartupPagesToCurrentPages(const ListValue* args);

  
  
  void RequestAutocompleteSuggestions(const ListValue* args);

  
  
  void ToggleShowBookmarksBar(const ListValue* args);

  
  int StatusStringIdForState(ShellIntegration::DefaultBrowserState state);

  
  
  void UpdateDefaultBrowserState();

  
  void SetDefaultBrowserUIString(int status_string_id);

  
  void UpdateStartupPages();

  
  void UpdateSearchEngines();

  
  void SaveStartupPagesPref();

  scoped_refptr<ShellIntegration::DefaultBrowserWorker> default_browser_worker_;

  StringPrefMember homepage_;
  BooleanPrefMember default_browser_policy_;

  TemplateURLModel* template_url_model_;  

  
  
  
  scoped_ptr<CustomHomePagesTableModel> startup_custom_pages_table_model_;
  scoped_ptr<OptionsManagedBannerHandler> banner_handler_;

  scoped_ptr<AutocompleteController> autocomplete_controller_;

  DISALLOW_COPY_AND_ASSIGN(BrowserOptionsHandler);
};

#endif  
