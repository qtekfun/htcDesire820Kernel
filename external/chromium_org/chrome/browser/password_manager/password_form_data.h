// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_FORM_DATA_H_
#define CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_FORM_DATA_H_

#include <ostream>

#include "components/autofill/core/common/password_form.h"
#include "testing/gmock/include/gmock/gmock.h"


struct PasswordFormData {
  const autofill::PasswordForm::Scheme scheme;
  const char* signon_realm;
  const char* origin;
  const char* action;
  const wchar_t* submit_element;
  const wchar_t* username_element;
  const wchar_t* password_element;
  const wchar_t* username_value;  
  const wchar_t* password_value;
  const bool preferred;
  const bool ssl_valid;
  const double creation_time;
};

autofill::PasswordForm* CreatePasswordFormFromData(
    const PasswordFormData& form_data);

bool ContainsSamePasswordFormsPtr(
    const std::vector<autofill::PasswordForm*>& first,
    const std::vector<autofill::PasswordForm*>& second);

bool ContainsSamePasswordForms(
    std::vector<autofill::PasswordForm>& first,
    std::vector<autofill::PasswordForm>& second);

MATCHER_P(ContainsAllPasswordForms, forms, "") {
  return ContainsSamePasswordFormsPtr(forms, arg);
}

#endif  
