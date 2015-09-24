// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_CLEAR_BROWSER_DATA_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_CLEAR_BROWSER_DATA_HANDLER_H_
#pragma once

#include "chrome/browser/browsing_data_remover.h"
#include "chrome/browser/prefs/pref_member.h"
#include "chrome/browser/ui/webui/options/options_ui.h"

class ClearBrowserDataHandler : public OptionsPageUIHandler,
                                public BrowsingDataRemover::Observer {
 public:
  ClearBrowserDataHandler();
  virtual ~ClearBrowserDataHandler();

  
  virtual void Initialize();

  virtual void GetLocalizedValues(DictionaryValue* localized_strings);

  
  virtual void RegisterMessages();

 private:
  
  void HandleClearBrowserData(const ListValue* value);

  
  virtual void OnBrowsingDataRemoverDone();

  
  
  BrowsingDataRemover* remover_;

  
  BooleanPrefMember clear_plugin_lso_data_enabled_;

  DISALLOW_COPY_AND_ASSIGN(ClearBrowserDataHandler);
};

#endif  
