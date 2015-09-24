// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOFILL_AUTOFILL_COUNTRY_H_
#define CHROME_BROWSER_AUTOFILL_AUTOFILL_COUNTRY_H_
#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/string16.h"

class AutofillCountry {
 public:
  
  
  AutofillCountry(const std::string& country_code, const std::string& locale);
  ~AutofillCountry();

  
  static void GetAvailableCountries(
      std::vector<std::string>* country_codes);

  
  
  static const std::string CountryCodeForLocale(const std::string& locale);

  
  
  
  static const std::string GetCountryCode(const string16& country,
                                          const std::string& locale);

  
  static const std::string ApplicationLocale();

  const std::string country_code() const { return country_code_; }
  const string16 name() const { return name_; }
  const string16 postal_code_label() const { return postal_code_label_; }
  const string16 state_label() const { return state_label_; }

 private:
  AutofillCountry(const std::string& country_code,
                  const string16& name,
                  const string16& postal_code_label,
                  const string16& state_label);

  
  std::string country_code_;

  
  string16 name_;

  
  string16 postal_code_label_;

  
  string16 state_label_;

  DISALLOW_COPY_AND_ASSIGN(AutofillCountry);
};

#endif  
