// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOFILL_CREDIT_CARD_H_
#define CHROME_BROWSER_AUTOFILL_CREDIT_CARD_H_
#pragma once

#include <ostream>
#include <string>
#include <vector>

#include "base/string16.h"
#include "chrome/browser/autofill/field_types.h"
#include "chrome/browser/autofill/form_group.h"

class CreditCard : public FormGroup {
 public:
  explicit CreditCard(const std::string& guid);

  
  CreditCard();
  CreditCard(const CreditCard& credit_card);
  virtual ~CreditCard();

  
  virtual void GetPossibleFieldTypes(const string16& text,
                                     FieldTypeSet* possible_types) const;
  virtual void GetAvailableFieldTypes(FieldTypeSet* available_types) const;
  virtual string16 GetInfo(AutofillFieldType type) const;
  virtual void SetInfo(AutofillFieldType type, const string16& value);
  
  virtual const string16 Label() const;

  
  void SetInfoForMonthInputType(const string16& value);

  
  string16 ObfuscatedNumber() const;
  
  string16 LastFourDigits() const;

  const std::string& type() const { return type_; }

  
  const std::string guid() const { return guid_; }
  void set_guid(const std::string& guid) { guid_ = guid; }

  
  void operator=(const CreditCard& credit_card);

  
  
  
  
  
  
  int Compare(const CreditCard& credit_card) const;

  
  bool operator==(const CreditCard& credit_card) const;
  bool operator!=(const CreditCard& credit_card) const;

  
  static const string16 StripSeparators(const string16& number);

  
  
  static bool IsValidCreditCardNumber(const string16& text);

  
  bool IsEmpty() const;

  
  const string16& number() const { return number_; }

 private:
  
  int Expiration4DigitYear() const { return expiration_year_; }
  int Expiration2DigitYear() const { return expiration_year_ % 100; }
  string16 ExpirationMonthAsString() const;
  string16 Expiration4DigitYearAsString() const;
  string16 Expiration2DigitYearAsString() const;

  
  void SetExpirationMonthFromString(const string16& text);

  
  void SetExpirationYearFromString(const string16& text);

  
  void SetNumber(const string16& number);

  
  
  void SetExpirationMonth(int expiration_month);
  void SetExpirationYear(int expiration_year);

  
  
  bool IsNameOnCard(const string16& text) const;

  
  bool IsNumber(const string16& text) const;

  
  bool IsExpirationMonth(const string16& text) const;

  
  
  bool Is2DigitExpirationYear(const string16& text) const;

  
  
  bool Is4DigitExpirationYear(const string16& text) const;

  string16 number_;  
  string16 name_on_card_;  
  std::string type_;  

  
  int expiration_month_;
  int expiration_year_;

  
  std::string guid_;
};

std::ostream& operator<<(std::ostream& os, const CreditCard& credit_card);

extern const char* const kAmericanExpressCard;
extern const char* const kDinersCard;
extern const char* const kDiscoverCard;
extern const char* const kGenericCard;
extern const char* const kJCBCard;
extern const char* const kMasterCard;
extern const char* const kSoloCard;
extern const char* const kVisaCard;

#endif  
