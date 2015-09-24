// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_COMMON_PASSWORD_FORM_H__
#define COMPONENTS_AUTOFILL_CORE_COMMON_PASSWORD_FORM_H__

#include <map>
#include <string>
#include <vector>

#include "base/time/time.h"
#include "components/autofill/core/common/form_data.h"
#include "url/gurl.h"

namespace autofill {


struct PasswordForm {
  
  
  
  enum Scheme {
    SCHEME_HTML,
    SCHEME_BASIC,
    SCHEME_DIGEST,
    SCHEME_OTHER
  } scheme;

  
  
  
  
  std::string signon_realm;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  std::string original_signon_realm;

  
  
  
  
  GURL origin;

  
  
  
  
  
  
  
  
  
  
  GURL action;

  
  
  
  
  
  base::string16 submit_element;

  
  
  
  base::string16 username_element;

  
  
  
  
  base::string16 username_value;

  
  
  
  
  
  std::vector<base::string16> other_possible_usernames;

  
  
  
  base::string16 password_element;

  
  
  
  base::string16 password_value;

  
  
  bool password_autocomplete_set;

  
  
  base::string16 old_password_element;

  
  base::string16 old_password_value;

  
  
  
  
  
  
  
  bool ssl_valid;

  
  
  
  
  
  
  
  bool preferred;

  
  
  
  base::Time date_created;

  
  
  
  
  bool blacklisted_by_user;

  
  
  enum Type {
    TYPE_MANUAL,
    TYPE_GENERATED,
  };

  
  Type type;

  
  
  
  
  int times_used;

  
  
  
  
  
  FormData form_data;

  
  bool IsPublicSuffixMatch() const;

  
  bool operator==(const PasswordForm& form) const;
  bool operator!=(const PasswordForm& form) const;

  PasswordForm();
  ~PasswordForm();
};

typedef std::map<base::string16, PasswordForm*> PasswordFormMap;

std::ostream& operator<<(std::ostream& os,
                         const autofill::PasswordForm& form);

}  

#endif  
