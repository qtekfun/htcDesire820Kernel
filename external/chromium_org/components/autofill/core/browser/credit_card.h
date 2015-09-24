// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_CREDIT_CARD_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_CREDIT_CARD_H_

#include <iosfwd>
#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/strings/string16.h"
#include "components/autofill/core/browser/autofill_data_model.h"

namespace autofill {

struct FormFieldData;

class CreditCard : public AutofillDataModel {
 public:
  CreditCard(const std::string& guid, const std::string& origin);

  
  CreditCard();
  CreditCard(const CreditCard& credit_card);
  virtual ~CreditCard();

  
  static const base::string16 StripSeparators(const base::string16& number);

  
  static base::string16 TypeForDisplay(const std::string& type);

  
  static int IconResourceId(const std::string& type);

  
  
  
  
  
  
  
  static std::string GetCreditCardType(const base::string16& number);

  
  virtual void GetMatchingTypes(
      const base::string16& text,
      const std::string& app_locale,
      ServerFieldTypeSet* matching_types) const OVERRIDE;
  virtual base::string16 GetRawInfo(ServerFieldType type) const OVERRIDE;
  virtual void SetRawInfo(ServerFieldType type,
                          const base::string16& value) OVERRIDE;
  virtual base::string16 GetInfo(const AutofillType& type,
                                 const std::string& app_locale) const OVERRIDE;
  virtual bool SetInfo(const AutofillType& type,
                       const base::string16& value,
                       const std::string& app_locale) OVERRIDE;

  
  const base::string16 Label() const;

  
  void SetInfoForMonthInputType(const base::string16& value);

  
  base::string16 ObfuscatedNumber() const;
  
  base::string16 LastFourDigits() const;
  
  base::string16 TypeForDisplay() const;
  
  base::string16 TypeAndLastFourDigits() const;

  const std::string& type() const { return type_; }

  int expiration_month() const { return expiration_month_; }
  int expiration_year() const { return expiration_year_; }

  
  void operator=(const CreditCard& credit_card);

  
  
  
  
  bool UpdateFromImportedCard(const CreditCard& imported_card,
                              const std::string& app_locale) WARN_UNUSED_RESULT;

  
  
  
  
  
  
  int Compare(const CreditCard& credit_card) const;

  
  bool operator==(const CreditCard& credit_card) const;
  bool operator!=(const CreditCard& credit_card) const;

  
  bool IsEmpty(const std::string& app_locale) const;

  
  bool IsComplete() const;

  
  
  bool IsValid() const;

  
  const base::string16& number() const { return number_; }

 private:
  
  virtual void GetSupportedTypes(
      ServerFieldTypeSet* supported_types) const OVERRIDE;

  
  int Expiration4DigitYear() const { return expiration_year_; }
  int Expiration2DigitYear() const { return expiration_year_ % 100; }
  base::string16 ExpirationMonthAsString() const;
  base::string16 Expiration4DigitYearAsString() const;
  base::string16 Expiration2DigitYearAsString() const;

  
  void SetExpirationMonthFromString(const base::string16& text,
                                    const std::string& app_locale);

  
  void SetExpirationYearFromString(const base::string16& text);

  
  void SetNumber(const base::string16& number);

  
  
  void SetExpirationMonth(int expiration_month);
  void SetExpirationYear(int expiration_year);

  base::string16 number_;  
  base::string16 name_on_card_;  
  std::string type_;  

  
  int expiration_month_;
  int expiration_year_;
};

std::ostream& operator<<(std::ostream& os, const CreditCard& credit_card);

extern const char* const kAmericanExpressCard;
extern const char* const kDinersCard;
extern const char* const kDiscoverCard;
extern const char* const kGenericCard;
extern const char* const kJCBCard;
extern const char* const kMasterCard;
extern const char* const kUnionPay;
extern const char* const kVisaCard;

}  

#endif  
