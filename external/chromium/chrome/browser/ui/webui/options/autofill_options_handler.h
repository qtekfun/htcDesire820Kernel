// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_AUTOFILL_OPTIONS_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_AUTOFILL_OPTIONS_HANDLER_H_

#include <string>

#include "chrome/browser/autofill/personal_data_manager.h"
#include "chrome/browser/ui/webui/options/options_ui.h"

class DictionaryValue;
class ListValue;

class AutofillOptionsHandler : public OptionsPageUIHandler,
                               public PersonalDataManager::Observer {
 public:
  AutofillOptionsHandler();
  virtual ~AutofillOptionsHandler();

  
  virtual void GetLocalizedValues(DictionaryValue* localized_strings);
  virtual void Initialize();
  virtual void RegisterMessages();

  
  virtual void OnPersonalDataLoaded();
  virtual void OnPersonalDataChanged();

 private:
  
  void SetAddressOverlayStrings(DictionaryValue* localized_strings);
  void SetCreditCardOverlayStrings(DictionaryValue* localized_strings);

  
  void LoadAutofillData();

  
  
  void RemoveAddress(const ListValue* args);

  
  
  void RemoveCreditCard(const ListValue* args);

  
  
  
  void LoadAddressEditor(const ListValue* args);

  
  
  
  void LoadCreditCardEditor(const ListValue* args);

  
  
  
  
  
  void SetAddress(const ListValue* args);

  
  
  
  
  
  void SetCreditCard(const ListValue* args);

  
  
  PersonalDataManager* personal_data_;

  DISALLOW_COPY_AND_ASSIGN(AutofillOptionsHandler);
};

#endif  
