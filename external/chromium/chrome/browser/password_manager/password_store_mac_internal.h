// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_STORE_MAC_INTERNAL_H_
#define CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_STORE_MAC_INTERNAL_H_
#pragma once

#include <Security/Security.h>

#include <string>
#include <vector>

#include "base/time.h"
#include "chrome/browser/keychain_mac.h"

class MacKeychainPasswordFormAdapter {
 public:
  
  
  
  explicit MacKeychainPasswordFormAdapter(const MacKeychain* keychain);

  
  
  std::vector<webkit_glue::PasswordForm*> PasswordsFillingForm(
      const webkit_glue::PasswordForm& query_form);

  
  
  
  std::vector<webkit_glue::PasswordForm*> PasswordsMergeableWithForm(
      const webkit_glue::PasswordForm& query_form);

  
  
  
  
  webkit_glue::PasswordForm* PasswordExactlyMatchingForm(
      const webkit_glue::PasswordForm& query_form);

  
  
  
  
  bool HasPasswordsMergeableWithForm(
      const webkit_glue::PasswordForm& query_form);

  
  std::vector<webkit_glue::PasswordForm*> GetAllPasswordFormPasswords();

  
  
  
  bool AddPassword(const webkit_glue::PasswordForm& form);

  
  
  bool RemovePassword(const webkit_glue::PasswordForm& form);

  
  
  void SetFindsOnlyOwnedItems(bool finds_only_owned);

 private:
  
  
  
  std::vector<webkit_glue::PasswordForm*> ConvertKeychainItemsToForms(
      std::vector<SecKeychainItemRef>* items);

  
  
  
  SecKeychainItemRef KeychainItemForForm(
      const webkit_glue::PasswordForm& form);

  
  
  
  
  std::vector<SecKeychainItemRef> MatchingKeychainItems(
      const std::string& signon_realm, webkit_glue::PasswordForm::Scheme scheme,
      const char* path, const char* username);

  
  
  
  
  
  bool ExtractSignonRealmComponents(const std::string& signon_realm,
                                    std::string* server, int* port,
                                    bool* is_secure,
                                    std::string* security_domain);

  
  SecAuthenticationType AuthTypeForScheme(
      webkit_glue::PasswordForm::Scheme scheme);

  
  
  bool SetKeychainItemPassword(const SecKeychainItemRef& keychain_item,
                               const std::string& password);

  
  
  bool SetKeychainItemCreatorCode(const SecKeychainItemRef& keychain_item,
                                  OSType creator_code);

  
  
  
  
  
  OSType CreatorCodeForSearch();

  const MacKeychain* keychain_;

  
  bool finds_only_owned_;

  DISALLOW_COPY_AND_ASSIGN(MacKeychainPasswordFormAdapter);
};

namespace internal_keychain_helpers {

bool FillPasswordFormFromKeychainItem(const MacKeychain& keychain,
                                      const SecKeychainItemRef& keychain_item,
                                      webkit_glue::PasswordForm* form);

bool FormsMatchForMerge(const webkit_glue::PasswordForm& form_a,
                        const webkit_glue::PasswordForm& form_b);

void MergePasswordForms(std::vector<webkit_glue::PasswordForm*>* keychain_forms,
                        std::vector<webkit_glue::PasswordForm*>* database_forms,
                        std::vector<webkit_glue::PasswordForm*>* merged_forms);

std::vector<webkit_glue::PasswordForm*> GetPasswordsForForms(
    const MacKeychain& keychain,
    std::vector<webkit_glue::PasswordForm*>* database_forms);

}  

#endif  
