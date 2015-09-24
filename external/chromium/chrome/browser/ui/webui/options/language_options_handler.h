// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_LANGUAGE_OPTIONS_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_LANGUAGE_OPTIONS_HANDLER_H_
#pragma once

#include "chrome/browser/ui/webui/options/language_options_handler_common.h"

class LanguageOptionsHandler : public LanguageOptionsHandlerCommon {
 public:
  LanguageOptionsHandler();
  virtual ~LanguageOptionsHandler();

  
  virtual void GetLocalizedValues(DictionaryValue* localized_strings);

  
  virtual void RegisterMessages();

  

  
  
  
  
  static ListValue* GetLanguageList();

 private:
  
  virtual string16 GetProductName();
  virtual void SetApplicationLocale(const std::string& language_code);

  
  void RestartCallback(const ListValue* args);

  DISALLOW_COPY_AND_ASSIGN(LanguageOptionsHandler);
};

#endif  
