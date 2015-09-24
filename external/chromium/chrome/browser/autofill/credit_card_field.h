// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOFILL_CREDIT_CARD_FIELD_H_
#define CHROME_BROWSER_AUTOFILL_CREDIT_CARD_FIELD_H_
#pragma once

#include <vector>

#include "base/basictypes.h"
#include "chrome/browser/autofill/autofill_type.h"
#include "chrome/browser/autofill/form_field.h"

class AutofillField;

class CreditCardField : public FormField {
 public:
  
  virtual bool GetFieldInfo(FieldTypeMap* field_type_map) const;
  virtual FormFieldType GetFormFieldType() const;

  static CreditCardField* Parse(
      std::vector<AutofillField*>::const_iterator* iter,
      bool is_ecml);

 private:
  CreditCardField();

  AutofillField* cardholder_;  

  
  
  
  
  
  
  
  AutofillField* cardholder_last_;

  AutofillField* type_;  
  AutofillField* number_;  

  
  AutofillField* verification_;

  
  AutofillField* expiration_month_;
  AutofillField* expiration_year_;

  DISALLOW_COPY_AND_ASSIGN(CreditCardField);
};

#endif  
