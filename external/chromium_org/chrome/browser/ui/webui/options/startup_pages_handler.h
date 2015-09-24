// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_STARTUP_PAGES_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_STARTUP_PAGES_HANDLER_H_

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/prefs/pref_change_registrar.h"
#include "base/prefs/pref_member.h"
#include "chrome/browser/autocomplete/autocomplete_controller_delegate.h"
#include "chrome/browser/ui/webui/options/options_ui.h"
#include "ui/base/models/table_model_observer.h"

class AutocompleteController;
class CustomHomePagesTableModel;
class TemplateURLService;

namespace options {

class StartupPagesHandler : public OptionsPageUIHandler,
                            public AutocompleteControllerDelegate,
                            public ui::TableModelObserver {
 public:
  StartupPagesHandler();
  virtual ~StartupPagesHandler();

  
  virtual void GetLocalizedValues(DictionaryValue* localized_strings) OVERRIDE;
  virtual void InitializeHandler() OVERRIDE;
  virtual void InitializePage() OVERRIDE;
  virtual void RegisterMessages() OVERRIDE;

  
  virtual void OnResultChanged(bool default_match_changed) OVERRIDE;

  
  virtual void OnModelChanged() OVERRIDE;
  virtual void OnItemsChanged(int start, int length) OVERRIDE;
  virtual void OnItemsAdded(int start, int length) OVERRIDE;
  virtual void OnItemsRemoved(int start, int length) OVERRIDE;

 private:
  
  void CommitChanges(const ListValue* args);

  
  void CancelChanges(const ListValue* args);

  
  void RemoveStartupPages(const ListValue* args);

  
  
  void AddStartupPage(const ListValue* args);

  
  
  void EditStartupPage(const ListValue* args);

  
  void SetStartupPagesToCurrentPages(const ListValue* args);

  
  void DragDropStartupPage(const ListValue* args);

  
  
  void RequestAutocompleteSuggestions(const ListValue* args);

  
  void UpdateStartupPages();

  
  void SaveStartupPagesPref();

  scoped_ptr<AutocompleteController> autocomplete_controller_;

  
  
  PrefChangeRegistrar pref_change_registrar_;

  
  
  
  scoped_ptr<CustomHomePagesTableModel> startup_custom_pages_table_model_;

  DISALLOW_COPY_AND_ASSIGN(StartupPagesHandler);
};

}  

#endif  
