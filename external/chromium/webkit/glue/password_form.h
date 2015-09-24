// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_GLUE_PASSWORD_FORM_H__
#define WEBKIT_GLUE_PASSWORD_FORM_H__

#include <string>
#include <map>

#include "base/time.h"
#include "googleurl/src/gurl.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebPasswordFormData.h"

namespace webkit_glue {


struct PasswordForm {
  
  
  
  enum Scheme {
    SCHEME_HTML,
    SCHEME_BASIC,
    SCHEME_DIGEST,
    SCHEME_OTHER
  } scheme;

  
  
  
  
  std::string signon_realm;

  
  
  
  
  GURL origin;

  
  
  
  
  
  
  
  
  
  
  GURL action;

  
  
  
  
  
  string16 submit_element;

  
  
  
  string16 username_element;

  
  
  
  
  string16 username_value;

  
  
  
  string16 password_element;

  
  
  
  string16 password_value;

  
  
  string16 old_password_element;

  
  string16 old_password_value;

  
  
  
  
  
  
  
  bool ssl_valid;

  
  
  
  
  
  
  
  bool preferred;

  
  
  
  base::Time date_created;

  
  
  
  
  bool blacklisted_by_user;

  PasswordForm();
  PasswordForm(const WebKit::WebPasswordFormData& web_password_form);
  ~PasswordForm();
};

typedef std::map<string16, PasswordForm*> PasswordFormMap;

}  

#endif  
