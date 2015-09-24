// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_CHROMEOS_LANGUAGE_HANGUL_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_CHROMEOS_LANGUAGE_HANGUL_HANDLER_H_
#pragma once

#include "chrome/browser/ui/webui/options/options_ui.h"

class DictionaryValue;
class ListValue;

namespace chromeos {

class LanguageHangulHandler : public OptionsPageUIHandler {
 public:
  LanguageHangulHandler();
  virtual ~LanguageHangulHandler();

  
  virtual void GetLocalizedValues(DictionaryValue* localized_strings);

 private:
  
  static ListValue* GetKeyboardLayoutList();

  DISALLOW_COPY_AND_ASSIGN(LanguageHangulHandler);
};

}  

#endif  
