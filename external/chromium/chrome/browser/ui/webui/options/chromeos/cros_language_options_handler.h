// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_CHROMEOS_CROS_LANGUAGE_OPTIONS_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_CHROMEOS_CROS_LANGUAGE_OPTIONS_HANDLER_H_
#pragma once

#include "chrome/browser/chromeos/input_method/input_method_util.h"
#include "chrome/browser/ui/webui/options/language_options_handler.h"

namespace chromeos {

class CrosLanguageOptionsHandler : public LanguageOptionsHandlerCommon {
 public:
  CrosLanguageOptionsHandler();
  virtual ~CrosLanguageOptionsHandler();

  
  virtual void GetLocalizedValues(DictionaryValue* localized_strings);

  
  virtual void RegisterMessages();

  

  
  
  
  
  
  
  
  static ListValue* GetInputMethodList(
      const chromeos::InputMethodDescriptors& descriptors);

  
  
  
  
  static ListValue* GetLanguageList(
      const chromeos::InputMethodDescriptors& descriptors);

 private:
  
  virtual string16 GetProductName();
  virtual void SetApplicationLocale(const std::string& language_code);

  
  void RestartCallback(const ListValue* args);

  
  
  void InputMethodDisableCallback(const ListValue* args);

  
  
  void InputMethodEnableCallback(const ListValue* args);

  
  
  void InputMethodOptionsOpenCallback(const ListValue* args);

  DISALLOW_COPY_AND_ASSIGN(CrosLanguageOptionsHandler);
};

} 

#endif  
