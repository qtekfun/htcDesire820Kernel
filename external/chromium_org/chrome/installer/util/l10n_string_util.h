// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_L10N_STRING_UTIL_H_
#define CHROME_INSTALLER_UTIL_L10N_STRING_UTIL_H_

#include <string>

#include "base/strings/string16.h"

namespace installer {

class TranslationDelegate {
 public:
  virtual ~TranslationDelegate();
  virtual string16 GetLocalizedString(int installer_string_id) = 0;
};

void SetTranslationDelegate(TranslationDelegate* delegate);

std::wstring GetLocalizedString(int base_message_id);

std::wstring GetLocalizedEulaResource();

std::wstring GetCurrentTranslation();

}  

#endif  
