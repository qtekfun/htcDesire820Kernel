// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_TEST_INTEGRATION_AUTOFILL_HELPER_H_
#define CHROME_BROWSER_SYNC_TEST_INTEGRATION_AUTOFILL_HELPER_H_

#include <set>
#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/strings/string16.h"
#include "chrome/browser/sync/test/integration/sync_datatype_helper.h"

namespace autofill {
class AutofillEntry;
class AutofillKey;
class AutofillProfile;
class AutofillType;
class AutofillWebDataService;
class CreditCard;
class PersonalDataManager;
}  

namespace autofill_helper {

enum ProfileType {
  PROFILE_MARION,
  PROFILE_HOMER,
  PROFILE_FRASIER,
  PROFILE_NULL
};

scoped_refptr<autofill::AutofillWebDataService> GetWebDataService(
    int index) WARN_UNUSED_RESULT;

autofill::PersonalDataManager* GetPersonalDataManager(
    int index) WARN_UNUSED_RESULT;

void AddKeys(int profile, const std::set<autofill::AutofillKey>& keys);

void RemoveKey(int profile, const autofill::AutofillKey& key);

void RemoveKeys(int profile);

std::set<autofill::AutofillEntry> GetAllKeys(int profile) WARN_UNUSED_RESULT;

bool KeysMatch(int profile_a, int profile_b) WARN_UNUSED_RESULT;

void SetProfiles(int profile,
                 std::vector<autofill::AutofillProfile>* autofill_profiles);

void SetCreditCards(int profile,
                    std::vector<autofill::CreditCard>* credit_cards);

void AddProfile(int profile, const autofill::AutofillProfile& autofill_profile);

void RemoveProfile(int profile, const std::string& guid);

void UpdateProfile(int profile,
                   const std::string& guid,
                   const autofill::AutofillType& type,
                   const base::string16& value);

const std::vector<autofill::AutofillProfile*>& GetAllProfiles(
    int profile) WARN_UNUSED_RESULT;

int GetProfileCount(int profile);

int GetKeyCount(int profile);

bool ProfilesMatch(int profile_a, int profile_b) WARN_UNUSED_RESULT;

bool AllProfilesMatch() WARN_UNUSED_RESULT;

autofill::AutofillProfile CreateAutofillProfile(ProfileType type);

}  

#endif  
