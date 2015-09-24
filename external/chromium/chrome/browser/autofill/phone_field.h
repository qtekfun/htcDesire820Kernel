// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOFILL_PHONE_FIELD_H_
#define CHROME_BROWSER_AUTOFILL_PHONE_FIELD_H_
#pragma once

#include <vector>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/autofill/autofill_type.h"
#include "chrome/browser/autofill/form_field.h"
#include "chrome/browser/autofill/phone_number.h"

class AutofillField;

class PhoneField : public FormField {
 public:
  virtual ~PhoneField();

  static PhoneField* Parse(std::vector<AutofillField*>::const_iterator* iter,
                           bool is_ecml);
  static PhoneField* ParseECML(
      std::vector<AutofillField*>::const_iterator* iter);

  virtual bool GetFieldInfo(FieldTypeMap* field_type_map) const;

 private:
  PhoneField();

  enum PhoneType {
    PHONE_TYPE_FIRST = 0,
    HOME_PHONE = PHONE_TYPE_FIRST,
    FAX_PHONE,

    
    PHONE_TYPE_MAX,
  };

  
  string16 GetCountryRegex() const;
  
  string16 GetAreaRegex() const;
  
  
  
  string16 GetAreaNoTextRegex() const;
  string16 GetPhoneRegex() const;
  string16 GetPrefixSeparatorRegex() const;
  string16 GetPrefixRegex() const;
  string16 GetSuffixSeparatorRegex() const;
  string16 GetSuffixRegex() const;
  string16 GetExtensionRegex() const;

  
  
  enum RegexType {
    REGEX_COUNTRY,
    REGEX_AREA,
    REGEX_AREA_NOTEXT,
    REGEX_PHONE,
    REGEX_PREFIX_SEPARATOR,
    REGEX_PREFIX,
    REGEX_SUFFIX_SEPARATOR,
    REGEX_SUFFIX,
    REGEX_EXTENSION,

    
    REGEX_SEPARATOR,
  };

  string16 GetRegExp(RegexType regex_id) const;

  
  
  
  
  
  
  static bool ParseInternal(PhoneField* field,
                            std::vector<AutofillField*>::const_iterator* iter,
                            bool regular_phone);

  void SetPhoneType(PhoneType phone_type);

  
  
  scoped_ptr<PhoneNumber> number_;
  PhoneType phone_type_;


  
  enum PhonePart {
    FIELD_NONE = -1,
    FIELD_COUNTRY_CODE,
    FIELD_AREA_CODE,
    FIELD_PHONE,
    FIELD_SUFFIX,
    FIELD_EXTENSION,

    FIELD_MAX,
  };

  
  
  AutofillField* parsed_phone_fields_[FIELD_MAX];

  static struct Parser {
    RegexType regex;       
    PhonePart phone_part;  
    int max_size;          
  } phone_field_grammars_[];

  DISALLOW_COPY_AND_ASSIGN(PhoneField);
};

#endif  
