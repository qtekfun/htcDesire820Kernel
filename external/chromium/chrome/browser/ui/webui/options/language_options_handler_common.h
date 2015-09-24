// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_LANGUAGE_OPTIONS_HANDLER_COMMON_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_LANGUAGE_OPTIONS_HANDLER_COMMON_H_
#pragma once

#include "chrome/browser/ui/webui/options/options_ui.h"

class DictionaryValue;
class ListValue;

class LanguageOptionsHandlerCommon : public OptionsPageUIHandler {
 public:
  LanguageOptionsHandlerCommon();
  virtual ~LanguageOptionsHandlerCommon();

  
  virtual void GetLocalizedValues(DictionaryValue* localized_strings);

  
  virtual void RegisterMessages();

  

  
  
  
  
  
  
  static DictionaryValue* GetUILanguageCodeSet();

  
  
  
  
  
  
  static DictionaryValue* GetSpellCheckLanguageCodeSet();

 private:
  
  virtual string16 GetProductName() = 0;

  
  virtual void SetApplicationLocale(const std::string& language_code) = 0;

  
  void LanguageOptionsOpenCallback(const ListValue* args);

  
  
  void UiLanguageChangeCallback(const ListValue* args);

  
  
  void SpellCheckLanguageChangeCallback(const ListValue* args);

  DISALLOW_COPY_AND_ASSIGN(LanguageOptionsHandlerCommon);
};

#endif  
