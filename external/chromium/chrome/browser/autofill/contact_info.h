// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOFILL_CONTACT_INFO_H_
#define CHROME_BROWSER_AUTOFILL_CONTACT_INFO_H_
#pragma once

#include <vector>

#include "base/gtest_prod_util.h"
#include "base/string16.h"
#include "chrome/browser/autofill/field_types.h"
#include "chrome/browser/autofill/form_group.h"

class NameInfo : public FormGroup {
 public:
  NameInfo();
  NameInfo(const NameInfo& info);
  virtual ~NameInfo();

  NameInfo& operator=(const NameInfo& info);

  
  virtual void GetPossibleFieldTypes(const string16& text,
                                     FieldTypeSet* possible_types) const;
  virtual void GetAvailableFieldTypes(FieldTypeSet* available_types) const;
  virtual string16 GetInfo(AutofillFieldType type) const;
  virtual void SetInfo(AutofillFieldType type, const string16& value);

 private:
  FRIEND_TEST_ALL_PREFIXES(NameInfoTest, TestSetFullName);

  
  
  string16 FullName() const;

  
  
  string16 MiddleInitial() const;

  const string16& first() const { return first_; }
  const string16& middle() const { return middle_; }
  const string16& last() const { return last_; }

  
  bool IsFirstName(const string16& text) const;

  
  bool IsMiddleName(const string16& text) const;

  
  bool IsLastName(const string16& text) const;

  
  bool IsMiddleInitial(const string16& text) const;

  
  bool IsFullName(const string16& text) const;

  
  
  bool IsNameMatch(const string16& text,
                   const std::vector<string16>& name_tokens) const;

  
  bool IsWordInName(const string16& word,
                    const std::vector<string16>& name_tokens) const;

  
  
  void SetFirst(const string16& first);

  
  
  void SetMiddle(const string16& middle);

  
  
  void SetLast(const string16& last);

  
  
  void SetFullName(const string16& full);

  
  std::vector<string16> first_tokens_;
  std::vector<string16> middle_tokens_;
  std::vector<string16> last_tokens_;

  string16 first_;
  string16 middle_;
  string16 last_;
};

class EmailInfo : public FormGroup {
 public:
  EmailInfo();
  EmailInfo(const EmailInfo& info);
  virtual ~EmailInfo();

  EmailInfo& operator=(const EmailInfo& info);

  
  virtual void GetPossibleFieldTypes(const string16& text,
                                     FieldTypeSet* possible_types) const;
  virtual void GetAvailableFieldTypes(FieldTypeSet* available_types) const;
  virtual string16 GetInfo(AutofillFieldType type) const;
  virtual void SetInfo(AutofillFieldType type, const string16& value);

 private:
  string16 email_;
};

class CompanyInfo : public FormGroup {
 public:
  CompanyInfo();
  CompanyInfo(const CompanyInfo& info);
  virtual ~CompanyInfo();

  CompanyInfo& operator=(const CompanyInfo& info);

  
  virtual void GetPossibleFieldTypes(const string16& text,
                                     FieldTypeSet* possible_types) const;
  virtual void GetAvailableFieldTypes(FieldTypeSet* available_types) const;
  virtual string16 GetInfo(AutofillFieldType type) const;
  virtual void SetInfo(AutofillFieldType type, const string16& value);

 private:
  string16 company_name_;
};

#endif  
