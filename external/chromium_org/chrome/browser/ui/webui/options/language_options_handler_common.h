// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_LANGUAGE_OPTIONS_HANDLER_COMMON_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_LANGUAGE_OPTIONS_HANDLER_COMMON_H_

#include "base/memory/weak_ptr.h"
#include "chrome/browser/spellchecker/spellcheck_hunspell_dictionary.h"
#include "chrome/browser/ui/webui/options/options_ui.h"

namespace base {
class DictionaryValue;
class ListValue;
}

namespace options {

class LanguageOptionsHandlerCommon
    : public OptionsPageUIHandler,
      public SpellcheckHunspellDictionary::Observer {
 public:
  LanguageOptionsHandlerCommon();
  virtual ~LanguageOptionsHandlerCommon();

  
  virtual void GetLocalizedValues(
      base::DictionaryValue* localized_strings) OVERRIDE;
  virtual void Uninitialize() OVERRIDE;

  
  virtual void RegisterMessages() OVERRIDE;

  
  virtual void OnHunspellDictionaryInitialized() OVERRIDE;
  virtual void OnHunspellDictionaryDownloadBegin() OVERRIDE;
  virtual void OnHunspellDictionaryDownloadSuccess() OVERRIDE;
  virtual void OnHunspellDictionaryDownloadFailure() OVERRIDE;

  

  
  
  
  
  
  
  static base::DictionaryValue* GetUILanguageCodeSet();

  
  
  
  
  
  
  static base::DictionaryValue* GetSpellCheckLanguageCodeSet();

 private:
  
  virtual base::string16 GetProductName() = 0;

  
  virtual void SetApplicationLocale(const std::string& language_code) = 0;

  
  void LanguageOptionsOpenCallback(const base::ListValue* args);

  
  
  void UiLanguageChangeCallback(const base::ListValue* args);

  
  
  void SpellCheckLanguageChangeCallback(const base::ListValue* args);

  
  
  void RetrySpellcheckDictionaryDownload(const base::ListValue* args);

  
  void UpdateLanguageListCallback(const base::ListValue* args);

  
  void RefreshHunspellDictionary();

  
  base::WeakPtr<SpellcheckHunspellDictionary>& GetHunspellDictionary();

  
  base::WeakPtr<SpellcheckHunspellDictionary> hunspell_dictionary_;

  DISALLOW_COPY_AND_ASSIGN(LanguageOptionsHandlerCommon);
};

}  

#endif  
