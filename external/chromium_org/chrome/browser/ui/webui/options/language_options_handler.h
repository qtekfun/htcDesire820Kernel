// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_LANGUAGE_OPTIONS_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_LANGUAGE_OPTIONS_HANDLER_H_

#include "chrome/browser/ui/webui/options/language_options_handler_common.h"

namespace options {

class LanguageOptionsHandler : public LanguageOptionsHandlerCommon {
 public:
  LanguageOptionsHandler();
  virtual ~LanguageOptionsHandler();

  
  virtual void GetLocalizedValues(
      base::DictionaryValue* localized_strings) OVERRIDE;

  
  virtual void RegisterMessages() OVERRIDE;

  

  
  
  
  
  static base::ListValue* GetLanguageList();

 private:
  
  virtual base::string16 GetProductName() OVERRIDE;
  virtual void SetApplicationLocale(const std::string& language_code) OVERRIDE;

  
  void RestartCallback(const base::ListValue* args);

  DISALLOW_COPY_AND_ASSIGN(LanguageOptionsHandler);
};

}  

#endif  
