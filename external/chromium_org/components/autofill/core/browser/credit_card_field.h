// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_CREDIT_CARD_FIELD_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_CREDIT_CARD_FIELD_H_

#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "components/autofill/core/browser/autofill_type.h"
#include "components/autofill/core/browser/form_field.h"

namespace autofill {

class AutofillField;
class AutofillScanner;

class CreditCardField : public FormField {
 public:
  static FormField* Parse(AutofillScanner* scanner);

 protected:
  
  virtual bool ClassifyField(ServerFieldTypeMap* map) const OVERRIDE;

 private:
  friend class CreditCardFieldTest;

  CreditCardField();

  const AutofillField* cardholder_;  

  
  
  
  
  
  
  
  const AutofillField* cardholder_last_;

  
  const AutofillField* type_;  
  const AutofillField* number_;  

  
  const AutofillField* verification_;

  
  
  const AutofillField* expiration_month_;
  const AutofillField* expiration_year_;
  const AutofillField* expiration_date_;

  
  
  bool is_two_digit_year_;

  DISALLOW_COPY_AND_ASSIGN(CreditCardField);
};

}  

#endif  
