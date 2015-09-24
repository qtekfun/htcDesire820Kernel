// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_SEARCH_ENGINE_MANAGER_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_SEARCH_ENGINE_MANAGER_HANDLER_H_

#include "chrome/browser/ui/search_engines/edit_search_engine_controller.h"
#include "chrome/browser/ui/webui/options/options_ui.h"
#include "ui/base/models/table_model_observer.h"

class KeywordEditorController;

class SearchEngineManagerHandler : public OptionsPageUIHandler,
                                   public ui::TableModelObserver,
                                   public EditSearchEngineControllerDelegate {
 public:
  SearchEngineManagerHandler();
  virtual ~SearchEngineManagerHandler();

  virtual void Initialize();

  
  virtual void GetLocalizedValues(DictionaryValue* localized_strings);

  
  virtual void OnModelChanged();
  virtual void OnItemsChanged(int start, int length);
  virtual void OnItemsAdded(int start, int length);
  virtual void OnItemsRemoved(int start, int length);

  
  virtual void OnEditedKeyword(const TemplateURL* template_url,
                               const string16& title,
                               const string16& keyword,
                               const std::string& url);

  virtual void RegisterMessages();

 private:
  scoped_ptr<KeywordEditorController> list_controller_;
  scoped_ptr<EditSearchEngineController> edit_controller_;

  
  void RemoveSearchEngine(const ListValue* args);

  
  void SetDefaultSearchEngine(const ListValue* args);

  
  
  
  void EditSearchEngine(const ListValue* args);

  
  
  void CheckSearchEngineInfoValidity(const ListValue* args);

  
  
  void EditCancelled(const ListValue* args);

  
  
  void EditCompleted(const ListValue* args);

  
  DictionaryValue* CreateDictionaryForEngine(int index, bool is_default);

  DISALLOW_COPY_AND_ASSIGN(SearchEngineManagerHandler);
};

#endif  
