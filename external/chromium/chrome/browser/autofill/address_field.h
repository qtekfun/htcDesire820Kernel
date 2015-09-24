// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOFILL_ADDRESS_FIELD_H_
#define CHROME_BROWSER_AUTOFILL_ADDRESS_FIELD_H_
#pragma once

#include <vector>

#include "base/basictypes.h"
#include "base/string16.h"
#include "chrome/browser/autofill/autofill_type.h"
#include "chrome/browser/autofill/field_types.h"
#include "chrome/browser/autofill/form_field.h"

class AutofillField;

class AddressField : public FormField {
 public:
  virtual bool GetFieldInfo(FieldTypeMap* field_type_map) const;
  virtual FormFieldType GetFormFieldType() const;

  static AddressField* Parse(std::vector<AutofillField*>::const_iterator* iter,
                             bool is_ecml);

  
  AddressType FindType() const;

  void SetType(AddressType address_type) { type_ = address_type; }

  
  
  bool IsFullAddress();

 private:
  AddressField();

  static bool ParseCompany(std::vector<AutofillField*>::const_iterator* iter,
                           bool is_ecml, AddressField* address_field);
  static bool ParseAddressLines(
      std::vector<AutofillField*>::const_iterator* iter,
      bool is_ecml, AddressField* address_field);
  static bool ParseCountry(std::vector<AutofillField*>::const_iterator* iter,
                           bool is_ecml, AddressField* address_field);
  static bool ParseZipCode(std::vector<AutofillField*>::const_iterator* iter,
                           bool is_ecml, AddressField* address_field);
  static bool ParseCity(std::vector<AutofillField*>::const_iterator* iter,
                        bool is_ecml, AddressField* address_field);
  static bool ParseState(std::vector<AutofillField*>::const_iterator* iter,
                         bool is_ecml, AddressField* address_field);

  
  
  static AddressType AddressTypeFromText(const string16& text);

  AutofillField* company_;   
  AutofillField* address1_;
  AutofillField* address2_;  
  AutofillField* city_;
  AutofillField* state_;     
  AutofillField* zip_;
  AutofillField* zip4_;      
  AutofillField* country_;   

  AddressType type_;
  bool is_ecml_;

  DISALLOW_COPY_AND_ASSIGN(AddressField);
};

#endif  
