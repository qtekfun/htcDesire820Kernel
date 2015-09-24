// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_LANGUAGE_DICTIONARY_OVERLAY_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_LANGUAGE_DICTIONARY_OVERLAY_HANDLER_H_

#include "chrome/browser/spellchecker/spellcheck_custom_dictionary.h"
#include "chrome/browser/ui/webui/options/options_ui.h"

namespace options {

class LanguageDictionaryOverlayHandler
    : public OptionsPageUIHandler,
      public SpellcheckCustomDictionary::Observer {
 public:
  LanguageDictionaryOverlayHandler();
  virtual ~LanguageDictionaryOverlayHandler();

  
  virtual void GetLocalizedValues(
      base::DictionaryValue* localized_strings) OVERRIDE;
  virtual void RegisterMessages() OVERRIDE;
  virtual void Uninitialize() OVERRIDE;

  
  virtual void OnCustomDictionaryLoaded() OVERRIDE;
  virtual void OnCustomDictionaryChanged(
      const SpellcheckCustomDictionary::Change& dictionary_change) OVERRIDE;

 private:
  
  void ResetDictionaryWords();

  
  void RefreshWords(const base::ListValue* args);

  
  void AddWord(const base::ListValue* args);

  
  void RemoveWord(const base::ListValue* args);

  
  bool overlay_initialized_;

  
  
  SpellcheckCustomDictionary* dictionary_;

  DISALLOW_COPY_AND_ASSIGN(LanguageDictionaryOverlayHandler);
};

}  

#endif  
