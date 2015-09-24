// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_PASSWORD_AUTOFILL_MANAGER_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_PASSWORD_AUTOFILL_MANAGER_H_


#include <map>

#include "components/autofill/core/common/password_form_fill_data.h"

namespace autofill {

class AutofillDriver;

class PasswordAutofillManager {
 public:
  explicit PasswordAutofillManager(AutofillDriver* autofill_driver);
  virtual ~PasswordAutofillManager();

  
  
  bool DidAcceptAutofillSuggestion(const FormFieldData& field,
                                   const base::string16& username);

  
  void AddPasswordFormMapping(
      const FormFieldData& username_element,
      const PasswordFormFillData& password);

  
  void Reset();

 private:
  
  
  
  typedef std::map<FormFieldData,
                   PasswordFormFillData>
      LoginToPasswordInfoMap;

  
  
  bool WillFillUserNameAndPassword(
      const base::string16& current_username,
      const PasswordFormFillData& password);

  
  bool FindLoginInfo(const FormFieldData& field,
                     PasswordFormFillData* found_password);

  
  LoginToPasswordInfoMap login_to_password_info_;

  
  
  AutofillDriver* const autofill_driver_;  

  DISALLOW_COPY_AND_ASSIGN(PasswordAutofillManager);
};

}  

#endif  
