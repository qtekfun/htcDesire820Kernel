// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_AUTOFILL_TEST_UTILS_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_AUTOFILL_TEST_UTILS_H_

#include "base/memory/scoped_ptr.h"

class PrefService;

namespace content {
class BrowserContext;
}

namespace autofill {

class AutofillProfile;
class CreditCard;
struct FormData;
struct FormFieldData;

namespace test {

scoped_ptr<PrefService> PrefServiceForTesting();

void CreateTestFormField(const char* label,
                         const char* name,
                         const char* value,
                         const char* type,
                         FormFieldData* field);

void CreateTestAddressFormData(FormData* form);

AutofillProfile GetFullProfile();

AutofillProfile GetFullProfile2();

AutofillProfile GetVerifiedProfile();

AutofillProfile GetVerifiedProfile2();

CreditCard GetCreditCard();

CreditCard GetCreditCard2();

CreditCard GetVerifiedCreditCard();

CreditCard GetVerifiedCreditCard2();

void SetProfileInfo(AutofillProfile* profile,
    const char* first_name, const char* middle_name,
    const char* last_name, const char* email, const char* company,
    const char* address1, const char* address2, const char* city,
    const char* state, const char* zipcode, const char* country,
    const char* phone);

void SetProfileInfoWithGuid(AutofillProfile* profile,
    const char* guid, const char* first_name, const char* middle_name,
    const char* last_name, const char* email, const char* company,
    const char* address1, const char* address2, const char* city,
    const char* state, const char* zipcode, const char* country,
    const char* phone);

void SetCreditCardInfo(CreditCard* credit_card,
    const char* name_on_card, const char* card_number,
    const char* expiration_month, const char* expiration_year);

void DisableSystemServices(content::BrowserContext* browser_context);

}  
}  

#endif  
