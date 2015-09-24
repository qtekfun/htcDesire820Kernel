// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_GLUE_PASSWORD_FORM_DOM_MANAGER_H_
#define WEBKIT_GLUE_PASSWORD_FORM_DOM_MANAGER_H_

#include "webkit/glue/form_data.h"
#include "webkit/glue/password_form.h"

namespace WebKit {
class WebForm;
}

class GURL;

namespace webkit_glue {

struct PasswordFormFillData {
  typedef std::map<string16, string16> LoginCollection;

  FormData basic_data;
  LoginCollection additional_logins;
  bool wait_for_username;
  PasswordFormFillData();
  ~PasswordFormFillData();
};

class PasswordFormDomManager {
 public:
  
  
  
  
  static PasswordForm* CreatePasswordForm(const WebKit::WebFormElement& form);

  
  
  
  
  
  
  static void InitFillData(const PasswordForm& form_on_page,
                           const PasswordFormMap& matches,
                           const PasswordForm* const preferred_match,
                           bool wait_for_username_before_autofill,
                           PasswordFormFillData* result);
 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(PasswordFormDomManager);
};

}  

#endif  
