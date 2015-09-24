// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOFILL_FORM_FIELD_H_
#define CHROME_BROWSER_AUTOFILL_FORM_FIELD_H_
#pragma once

#include <vector>

#include "base/basictypes.h"
#include "base/string16.h"
#include "chrome/browser/autofill/autofill_type.h"

class AutofillField;
class FormStructure;

extern const char kEcmlShipToTitle[];
extern const char kEcmlShipToFirstName[];
extern const char kEcmlShipToMiddleName[];
extern const char kEcmlShipToLastName[];
extern const char kEcmlShipToNameSuffix[];
extern const char kEcmlShipToCompanyName[];
extern const char kEcmlShipToAddress1[];
extern const char kEcmlShipToAddress2[];
extern const char kEcmlShipToAddress3[];
extern const char kEcmlShipToCity[];
extern const char kEcmlShipToStateProv[];
extern const char kEcmlShipToPostalCode[];
extern const char kEcmlShipToCountry[];
extern const char kEcmlShipToPhone[];
extern const char kEcmlShipToEmail[];

extern const char kEcmlBillToTitle[];
extern const char kEcmlBillToFirstName[];
extern const char kEcmlBillToMiddleName[];
extern const char kEcmlBillToLastName[];
extern const char kEcmlBillToNameSuffix[];
extern const char kEcmlBillToCompanyName[];
extern const char kEcmlBillToAddress1[];
extern const char kEcmlBillToAddress2[];
extern const char kEcmlBillToAddress3[];
extern const char kEcmlBillToCity[];
extern const char kEcmlBillToStateProv[];
extern const char kEcmlBillToPostalCode[];
extern const char kEcmlBillToCountry[];
extern const char kEcmlBillToPhone[];
extern const char kEcmlBillToEmail[];

extern const char kEcmlCardHolder[];
extern const char kEcmlCardType[];
extern const char kEcmlCardNumber[];
extern const char kEcmlCardVerification[];
extern const char kEcmlCardExpireDay[];
extern const char kEcmlCardExpireMonth[];
extern const char kEcmlCardExpireYear[];

enum FormFieldType {
  kAddressType,
  kCreditCardType,
  kOtherFieldType
};

class FormField {
 public:
  virtual ~FormField() {}

  
  
  virtual bool GetFieldInfo(FieldTypeMap* field_type_map) const = 0;

  
  virtual FormFieldType GetFormFieldType() const;

  
  
  static bool Match(AutofillField* field,
                    const string16& pattern,
                    bool match_label_only);

  
  
  static FormField* ParseFormField(
      std::vector<AutofillField*>::const_iterator* field,
      bool is_ecml);

  
  
  static bool ParseText(std::vector<AutofillField*>::const_iterator* iter,
                        const string16& pattern);

  
  
  static bool ParseText(std::vector<AutofillField*>::const_iterator* iter,
                        const string16& pattern,
                        AutofillField** dest);

  
  
  static bool ParseEmptyText(std::vector<AutofillField*>::const_iterator* iter,
                             AutofillField** dest);

  
  
  static bool ParseLabelText(std::vector<AutofillField*>::const_iterator* iter,
                             const string16& pattern,
                             AutofillField** dest);

  
  static bool ParseEmpty(std::vector<AutofillField*>::const_iterator* iter);

  
  static bool Add(FieldTypeMap* field_type_map, AutofillField* field,
                  const AutofillType& type);

 protected:
  
  FormField() {}

  
  
  
  
  
  
  
  static string16 GetEcmlPattern(const char* ecml_name);
  static string16 GetEcmlPattern(const char* ecml_name1,
                                 const char* ecml_name2,
                                 char pattern_operator);

 private:
  static bool ParseText(std::vector<AutofillField*>::const_iterator* iter,
                        const string16& pattern,
                        AutofillField** dest,
                        bool match_label_only);

  
  static bool ParseLabelAndName(
      std::vector<AutofillField*>::const_iterator* iter,
      const string16& pattern,
      AutofillField** dest);

  DISALLOW_COPY_AND_ASSIGN(FormField);
};

class FormFieldSet : public std::vector<FormField*> {
 public:
  explicit FormFieldSet(FormStructure* form);

  ~FormFieldSet() {
    for (iterator i = begin(); i != end(); ++i)
      delete *i;
  }

 private:
  
  
  bool CheckECML(FormStructure* fields);

  DISALLOW_COPY_AND_ASSIGN(FormFieldSet);
};

namespace autofill {

bool MatchString(const string16& input, const string16& pattern);

}  

#endif  
