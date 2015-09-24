// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_PASSWORD_MANAGER_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_PASSWORD_MANAGER_HANDLER_H_

#include <string>
#include <vector>

#include "base/prefs/pref_member.h"
#include "chrome/browser/ui/passwords/password_manager_presenter.h"
#include "chrome/browser/ui/passwords/password_ui_view.h"
#include "chrome/browser/ui/webui/options/options_ui.h"

namespace options {

class PasswordManagerHandler : public OptionsPageUIHandler,
                               public PasswordUIView {
 public:
  PasswordManagerHandler();
  virtual ~PasswordManagerHandler();

  
  virtual void GetLocalizedValues(DictionaryValue* localized_strings) OVERRIDE;
  virtual void InitializeHandler() OVERRIDE;
  virtual void RegisterMessages() OVERRIDE;

  
  virtual Profile* GetProfile() OVERRIDE;
  virtual void ShowPassword(size_t index, const base::string16& password_value)
      OVERRIDE;
  virtual void SetPasswordList(
      const ScopedVector<autofill::PasswordForm>& password_list,
      bool show_passwords) OVERRIDE;
  virtual void SetPasswordExceptionList(
      const ScopedVector<autofill::PasswordForm>& password_exception_list)
      OVERRIDE;
#if !defined(OS_ANDROID)
  virtual gfx::NativeWindow GetNativeWindow() OVERRIDE;
#endif
 private:
  
  
  void HandleUpdatePasswordLists(const ListValue* args);

  
  
  void HandleRemoveSavedPassword(const ListValue* args);

  
  
  void HandleRemovePasswordException(const ListValue* args);

  
  
  void HandleRequestShowPassword(const ListValue* args);

  
  std::string languages_;

  
  PasswordManagerPresenter password_manager_presenter_;

  DISALLOW_COPY_AND_ASSIGN(PasswordManagerHandler);
};

}  

#endif  
