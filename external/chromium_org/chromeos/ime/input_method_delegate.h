// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_IME_INPUT_METHOD_DELEGATE_H_
#define CHROMEOS_IME_INPUT_METHOD_DELEGATE_H_

#include <string>

#include "base/basictypes.h"
#include "base/strings/string16.h"

namespace chromeos {
namespace input_method {

class InputMethodDelegate {
 public:
  InputMethodDelegate() {}
  virtual ~InputMethodDelegate() {}

  
  
  virtual std::string GetHardwareKeyboardLayout() const = 0;

  
  virtual string16 GetLocalizedString(int resource_id) const = 0;

  
  
  
  
  virtual string16 GetDisplayLanguageName(
      const std::string& language_code) const = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(InputMethodDelegate);
};

}  
}  

#endif  
