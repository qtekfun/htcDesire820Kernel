// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_AUTOFILL_COUNTRY_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_AUTOFILL_COUNTRY_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/strings/string16.h"

namespace autofill {

enum AddressRequiredFields {
  ADDRESS_REQUIRES_CITY  = 1 << 0,
  ADDRESS_REQUIRES_STATE = 1 << 1,
  ADDRESS_REQUIRES_ZIP   = 1 << 2,

  
  ADDRESS_REQUIRES_CITY_STATE =
    ADDRESS_REQUIRES_CITY | ADDRESS_REQUIRES_STATE,
  ADDRESS_REQUIRES_STATE_ZIP =
    ADDRESS_REQUIRES_STATE | ADDRESS_REQUIRES_ZIP,
  ADDRESS_REQUIRES_CITY_ZIP =
    ADDRESS_REQUIRES_CITY |ADDRESS_REQUIRES_ZIP,
  ADDRESS_REQUIRES_CITY_STATE_ZIP =
    ADDRESS_REQUIRES_CITY | ADDRESS_REQUIRES_STATE | ADDRESS_REQUIRES_ZIP,

  
  ADDRESS_REQUIRES_ADDRESS_LINE_1_ONLY = 0,

  
  
  ADDRESS_REQUIREMENTS_UNKNOWN = ADDRESS_REQUIRES_CITY_STATE_ZIP,
};

class AutofillCountry {
 public:
  
  
  AutofillCountry(const std::string& country_code, const std::string& locale);
  ~AutofillCountry();

  
  static void GetAvailableCountries(
      std::vector<std::string>* country_codes);

  
  
  static const std::string CountryCodeForLocale(const std::string& locale);

  
  
  
  static const std::string GetCountryCode(const base::string16& country,
                                          const std::string& locale);

  const std::string country_code() const { return country_code_; }
  const base::string16 name() const { return name_; }
  const base::string16 postal_code_label() const { return postal_code_label_; }
  const base::string16 state_label() const { return state_label_; }

  
  bool requires_city() const {
    return (address_required_fields_ & ADDRESS_REQUIRES_CITY) != 0;
  }

  
  bool requires_state() const {
    return (address_required_fields_ & ADDRESS_REQUIRES_STATE) != 0;
  }

  
  bool requires_zip() const {
    return (address_required_fields_ & ADDRESS_REQUIRES_ZIP) != 0;
  }

 private:
  AutofillCountry(const std::string& country_code,
                  const base::string16& name,
                  const base::string16& postal_code_label,
                  const base::string16& state_label);

  
  std::string country_code_;

  
  base::string16 name_;

  
  base::string16 postal_code_label_;

  
  base::string16 state_label_;

  
  AddressRequiredFields address_required_fields_;

  DISALLOW_COPY_AND_ASSIGN(AutofillCountry);
};

}  

#endif  
