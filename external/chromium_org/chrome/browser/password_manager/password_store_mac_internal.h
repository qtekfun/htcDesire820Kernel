// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_STORE_MAC_INTERNAL_H_
#define CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_STORE_MAC_INTERNAL_H_

#include <Security/Security.h>

#include <string>
#include <vector>

#include "components/autofill/core/common/password_form.h"
#include "crypto/apple_keychain.h"

using crypto::AppleKeychain;

class MacKeychainPasswordFormAdapter {
 public:
  
  
  
  explicit MacKeychainPasswordFormAdapter(const AppleKeychain* keychain);

  
  
  std::vector<autofill::PasswordForm*> PasswordsFillingForm(
      const autofill::PasswordForm& query_form);

  
  
  
  
  autofill::PasswordForm* PasswordExactlyMatchingForm(
      const autofill::PasswordForm& query_form);

  
  
  
  
  
  bool HasPasswordsMergeableWithForm(
      const autofill::PasswordForm& query_form);

  
  std::vector<SecKeychainItemRef> GetAllPasswordFormKeychainItems();

  
  
  std::vector<autofill::PasswordForm*> GetAllPasswordFormPasswords();

  
  
  
  bool AddPassword(const autofill::PasswordForm& form);

  
  
  bool RemovePassword(const autofill::PasswordForm& form);

  
  
  void SetFindsOnlyOwnedItems(bool finds_only_owned);

 private:
  
  
  
  std::vector<autofill::PasswordForm*> ConvertKeychainItemsToForms(
      std::vector<SecKeychainItemRef>* items);

  
  
  
  SecKeychainItemRef KeychainItemForForm(
      const autofill::PasswordForm& form);

  
  
  
  
  std::vector<SecKeychainItemRef> MatchingKeychainItems(
      const std::string& signon_realm,
      autofill::PasswordForm::Scheme scheme,
      const char* path,
      const char* username);

  
  SecAuthenticationType AuthTypeForScheme(
      autofill::PasswordForm::Scheme scheme);

  
  
  bool SetKeychainItemPassword(const SecKeychainItemRef& keychain_item,
                               const std::string& password);

  
  
  bool SetKeychainItemCreatorCode(const SecKeychainItemRef& keychain_item,
                                  OSType creator_code);

  
  
  
  
  
  OSType CreatorCodeForSearch();

  const AppleKeychain* keychain_;

  
  bool finds_only_owned_;

  DISALLOW_COPY_AND_ASSIGN(MacKeychainPasswordFormAdapter);
};

namespace internal_keychain_helpers {

typedef std::pair<SecKeychainItemRef*, autofill::PasswordForm*> ItemFormPair;

bool FillPasswordFormFromKeychainItem(const AppleKeychain& keychain,
                                      const SecKeychainItemRef& keychain_item,
                                      autofill::PasswordForm* form,
                                      bool extract_password_data);

bool FormsMatchForMerge(const autofill::PasswordForm& form_a,
                        const autofill::PasswordForm& form_b);

void MergePasswordForms(
    std::vector<autofill::PasswordForm*>* keychain_forms,
    std::vector<autofill::PasswordForm*>* database_forms,
    std::vector<autofill::PasswordForm*>* merged_forms);

std::vector<autofill::PasswordForm*> GetPasswordsForForms(
    const AppleKeychain& keychain,
    std::vector<autofill::PasswordForm*>* database_forms);

std::vector<ItemFormPair> ExtractAllKeychainItemAttributesIntoPasswordForms(
    std::vector<SecKeychainItemRef>* keychain_items,
    const AppleKeychain& keychain);

bool ExtractSignonRealmComponents(const std::string& signon_realm,
                                  std::string* server, int* port,
                                  bool* is_secure,
                                  std::string* security_domain);

bool FormIsValidAndMatchesOtherForm(const autofill::PasswordForm& query_form,
                                    const autofill::PasswordForm& other_form);

std::vector<autofill::PasswordForm*> ExtractPasswordsMergeableWithForm(
    const AppleKeychain& keychain,
    const std::vector<ItemFormPair>& item_form_pairs,
    const autofill::PasswordForm& query_form);

}  

#endif  
