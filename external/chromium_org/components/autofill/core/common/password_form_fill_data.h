// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_COMMON_PASSWORD_FORM_FILL_DATA_H_
#define COMPONENTS_AUTOFILL_CORE_COMMON_PASSWORD_FORM_FILL_DATA_H_

#include <map>

#include "base/memory/scoped_ptr.h"
#include "components/autofill/core/common/form_data.h"
#include "components/autofill/core/common/password_form.h"

namespace autofill {

struct UsernamesCollectionKey {
  UsernamesCollectionKey();
  ~UsernamesCollectionKey();

  
  bool operator<(const UsernamesCollectionKey& other) const;

  base::string16 username;
  base::string16 password;
  std::string realm;
};

struct PasswordAndRealm {
  base::string16 password;
  std::string realm;
};

struct PasswordFormFillData {
  typedef std::map<base::string16, PasswordAndRealm> LoginCollection;
  typedef std::map<UsernamesCollectionKey,
                   std::vector<base::string16> > UsernamesCollection;

  
  
  FormData basic_data;

  
  std::string preferred_realm;

  
  LoginCollection additional_logins;

  
  
  
  
  UsernamesCollection other_possible_usernames;

  
  
  
  
  
  bool wait_for_username;

  PasswordFormFillData();
  ~PasswordFormFillData();
};

void InitPasswordFormFillData(
    const PasswordForm& form_on_page,
    const PasswordFormMap& matches,
    const PasswordForm* const preferred_match,
    bool wait_for_username_before_autofill,
    bool enable_other_possible_usernames,
    PasswordFormFillData* result);

}  

#endif  
