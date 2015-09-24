// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_PASSWORDS_PASSWORD_UI_VIEW_H_
#define CHROME_BROWSER_UI_PASSWORDS_PASSWORD_UI_VIEW_H_

#include "base/memory/scoped_vector.h"
#include "ui/gfx/native_widget_types.h"

namespace autofill {
struct PasswordForm;
}

class Profile;

class PasswordUIView {
 public:
  virtual ~PasswordUIView() {}

  
  virtual Profile* GetProfile() = 0;

  
  
  
  virtual void ShowPassword(size_t index,
                            const base::string16& password_value) = 0;

  
  
  
  virtual void SetPasswordList(
      const ScopedVector<autofill::PasswordForm>& password_list,
      bool show_passwords) = 0;

  
  
  virtual void SetPasswordExceptionList(
      const ScopedVector<autofill::PasswordForm>& password_exception_list) = 0;
#if !defined(OS_ANDROID)
  
  virtual gfx::NativeWindow GetNativeWindow() = 0;
#endif
};

#endif  
