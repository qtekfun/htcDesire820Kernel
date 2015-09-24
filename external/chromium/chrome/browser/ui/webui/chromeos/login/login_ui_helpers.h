// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_CHROMEOS_LOGIN_LOGIN_UI_HELPERS_H_
#define CHROME_BROWSER_UI_WEBUI_CHROMEOS_LOGIN_LOGIN_UI_HELPERS_H_
#pragma once

#include <string>

#include "base/string_piece.h"

class DictionaryValue;
class FilePath;
class RefCountedBytes;

namespace chromeos {

class HTMLOperationsInterface {
 public:
  HTMLOperationsInterface() {}
  virtual ~HTMLOperationsInterface() {}

  virtual base::StringPiece GetLoginHTML();
  virtual std::string GetFullHTML(base::StringPiece login_html,
                                  DictionaryValue* localized_strings);
  virtual RefCountedBytes* CreateHTMLBytes(std::string full_html);

 private:
  DISALLOW_COPY_AND_ASSIGN(HTMLOperationsInterface);
};

}  

#endif  
