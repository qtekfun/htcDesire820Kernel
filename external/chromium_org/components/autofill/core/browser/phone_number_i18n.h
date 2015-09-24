// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_PHONE_NUMBER_I18N_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_PHONE_NUMBER_I18N_H_

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"

namespace i18n {
namespace phonenumbers {
class PhoneNumber;
}
}

namespace autofill {

namespace i18n {


bool ParsePhoneNumber(
    const base::string16& value,
    const std::string& region,
    base::string16* country_code,
    base::string16* city_code,
    base::string16* number,
    ::i18n::phonenumbers::PhoneNumber* i18n_number) WARN_UNUSED_RESULT;

base::string16 NormalizePhoneNumber(const base::string16& value,
                                    const std::string& region);

bool ConstructPhoneNumber(const base::string16& country_code,
                          const base::string16& city_code,
                          const base::string16& number,
                          const std::string& region,
                          base::string16* whole_number) WARN_UNUSED_RESULT;

bool PhoneNumbersMatch(const base::string16& number_a,
                       const base::string16& number_b,
                       const std::string& region,
                       const std::string& app_locale);

class PhoneObject {
 public:
  PhoneObject(const base::string16& number,
              const std::string& region);
  PhoneObject(const PhoneObject&);
  PhoneObject();
  ~PhoneObject();

  const std::string& region() const { return region_; }

  const base::string16& country_code() const { return country_code_; }
  const base::string16& city_code() const { return city_code_; }
  const base::string16& number() const { return number_; }

  const base::string16& GetFormattedNumber() const;
  base::string16 GetNationallyFormattedNumber() const;
  const base::string16& GetWholeNumber() const;

  PhoneObject& operator=(const PhoneObject& other);

  bool IsValidNumber() const { return i18n_number_ != NULL; }

 private:
  
  std::string region_;

  
  
  scoped_ptr< ::i18n::phonenumbers::PhoneNumber> i18n_number_;
  base::string16 city_code_;
  base::string16 country_code_;
  base::string16 number_;

  
  
  mutable base::string16 formatted_number_;

  
  
  mutable base::string16 whole_number_;
};

}  
}  

#endif  
