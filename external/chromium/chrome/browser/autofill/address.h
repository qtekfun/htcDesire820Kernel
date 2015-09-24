// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOFILL_ADDRESS_H_
#define CHROME_BROWSER_AUTOFILL_ADDRESS_H_
#pragma once

#include <string>
#include <vector>

#include "base/string16.h"
#include "chrome/browser/autofill/autofill_type.h"
#include "chrome/browser/autofill/field_types.h"
#include "chrome/browser/autofill/form_group.h"

class Address : public FormGroup {
 public:
  Address();
  Address(const Address& address);
  virtual ~Address();

  Address& operator=(const Address& address);

  
  virtual void GetPossibleFieldTypes(const string16& text,
                                     FieldTypeSet* possible_types) const;
  virtual void GetAvailableFieldTypes(FieldTypeSet* available_types) const;
  virtual string16 GetInfo(AutofillFieldType type) const;
  virtual void SetInfo(AutofillFieldType type, const string16& value);

  const std::string& country_code() const { return country_code_; }
  void set_country_code(const std::string& country_code) {
    country_code_ = country_code;
  }

  
  void Clear();

 private:
  
  typedef std::vector<string16> LineTokens;

  
  string16 Country() const;

  void set_line1(const string16& line1);
  void set_line2(const string16& line2);

  
  
  void SetCountry(const string16& country);

  
  
  virtual bool IsLine1(const string16& text) const;
  virtual bool IsLine2(const string16& text) const;
  virtual bool IsCity(const string16& text) const;
  virtual bool IsState(const string16& text) const;
  virtual bool IsCountry(const string16& text) const;
  virtual bool IsZipCode(const string16& text) const;

  
  
  bool IsLineMatch(const string16& text, const LineTokens& line_tokens) const;

  
  bool IsWordInLine(const string16& word, const LineTokens& line_tokens) const;

  
  LineTokens line1_tokens_;
  LineTokens line2_tokens_;

  
  string16 line1_;
  string16 line2_;
  string16 city_;
  string16 state_;
  std::string country_code_;
  string16 zip_code_;
};

#endif  
