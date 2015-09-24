// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_TEST_INTEGRATION_PASSWORDS_HELPER_H_
#define CHROME_BROWSER_SYNC_TEST_INTEGRATION_PASSWORDS_HELPER_H_

#include <vector>

#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/sync/profile_sync_service.h"
#include "chrome/browser/sync/test/integration/sync_test.h"
#include "components/autofill/core/common/password_form.h"

class PasswordStore;

namespace passwords_helper {

void AddLogin(PasswordStore* store, const autofill::PasswordForm& form);

void UpdateLogin(PasswordStore* store, const autofill::PasswordForm& form);

void GetLogins(PasswordStore* store,
               std::vector<autofill::PasswordForm>& matches);

void RemoveLogin(PasswordStore* store, const autofill::PasswordForm& form);

void RemoveLogins(PasswordStore* store);

void SetEncryptionPassphrase(int index,
                             const std::string& passphrase,
                             ProfileSyncService::PassphraseType type);

bool SetDecryptionPassphrase(int index, const std::string& passphrase);

PasswordStore* GetPasswordStore(int index);

PasswordStore* GetVerifierPasswordStore();

bool ProfileContainsSamePasswordFormsAsVerifier(int index);

bool ProfilesContainSamePasswordForms(int index_a, int index_b);

bool AllProfilesContainSamePasswordFormsAsVerifier();

bool AllProfilesContainSamePasswordForms();

int GetPasswordCount(int index);

int GetVerifierPasswordCount();

autofill::PasswordForm CreateTestPasswordForm(int index);

}  

#endif  
