// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_VALIDATION_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_VALIDATION_H_

#include "base/strings/string16.h"

namespace base {
class Time;
}  

namespace autofill {

bool IsValidCreditCardExpirationDate(const base::string16& year,
                                     const base::string16& month,
                                     const base::Time& now);
bool IsValidCreditCardExpirationDate(int year,
                                     int month,
                                     const base::Time& now);

bool IsValidCreditCardNumber(const base::string16& text);

bool IsValidCreditCardSecurityCode(const base::string16& text);

bool IsValidCreditCardSecurityCode(const base::string16& code,
                                   const base::string16& number);

bool IsValidEmailAddress(const base::string16& text);

bool IsValidState(const base::string16& text);

bool IsValidZip(const base::string16& text);

bool IsSSN(const base::string16& text);

}  

#endif  
