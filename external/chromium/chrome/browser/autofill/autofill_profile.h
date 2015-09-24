// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOFILL_AUTOFILL_PROFILE_H_
#define CHROME_BROWSER_AUTOFILL_AUTOFILL_PROFILE_H_
#pragma once

#include <stddef.h>
#include <list>
#include <ostream>
#include <string>
#include <vector>

#ifdef ANDROID
#include "base/base_api.h"
#endif
#include "base/string16.h"
#include "chrome/browser/autofill/address.h"
#include "chrome/browser/autofill/autofill_type.h"
#include "chrome/browser/autofill/contact_info.h"
#include "chrome/browser/autofill/fax_number.h"
#include "chrome/browser/autofill/field_types.h"
#include "chrome/browser/autofill/form_group.h"
#include "chrome/browser/autofill/home_phone_number.h"

class
#ifdef ANDROID
BASE_API
#endif
AutofillProfile : public FormGroup {
 public:
  explicit AutofillProfile(const std::string& guid);

  
  AutofillProfile();
  AutofillProfile(const AutofillProfile& profile);
  virtual ~AutofillProfile();

  AutofillProfile& operator=(const AutofillProfile& profile);

  
  virtual void GetPossibleFieldTypes(const string16& text,
                                     FieldTypeSet* possible_types) const;
  virtual void GetAvailableFieldTypes(FieldTypeSet* available_types) const;
  virtual string16 GetInfo(AutofillFieldType type) const;
  virtual void SetInfo(AutofillFieldType type, const string16& value);

  
  void SetMultiInfo(AutofillFieldType type,
                    const std::vector<string16>& values);
  void GetMultiInfo(AutofillFieldType type,
                    std::vector<string16>* values) const;

  
  static bool SupportsMultiValue(AutofillFieldType type);

  
  
  
  virtual const string16 Label() const;

  
  const std::string guid() const { return guid_; }
  void set_guid(const std::string& guid) { guid_ = guid; }

  
  const std::string CountryCode() const;
  void SetCountryCode(const std::string& country_code);

  
  
  
  
  
  
  
  
  
  
  
  
  
  static bool AdjustInferredLabels(std::vector<AutofillProfile*>* profiles);

  
  
  
  
  
  
  
  static void CreateInferredLabels(
      const std::vector<AutofillProfile*>* profiles,
      const std::vector<AutofillFieldType>* suggested_fields,
      AutofillFieldType excluded_field,
      size_t minimal_fields_shown,
      std::vector<string16>* created_labels);

  
  bool IsEmpty() const;

  
  
  
  
  
  
  
  int Compare(const AutofillProfile& profile) const;

  
  int CompareMulti(const AutofillProfile& profile) const;

  
  
  bool operator==(const AutofillProfile& profile) const;
  virtual bool operator!=(const AutofillProfile& profile) const;

  
  
  
  const string16 PrimaryValue() const;

  
  
  void OverwriteWithOrAddTo(const AutofillProfile& profile);

 private:
  typedef std::vector<const FormGroup*> FormGroupList;

  
  
  
  string16 ConstructInferredLabel(
      const std::vector<AutofillFieldType>& label_fields,
      size_t num_fields_to_include) const;

  
  
  
  
  
  static void CreateDifferentiatingLabels(
      const std::vector<AutofillProfile*>& profiles,
      const std::list<size_t>& indices,
      const std::vector<AutofillFieldType>& fields,
      size_t num_fields_to_include,
      std::vector<string16>* created_labels);

  
  
  FormGroupList FormGroups() const;
  const FormGroup* FormGroupForType(AutofillFieldType type) const;
  FormGroup* MutableFormGroupForType(AutofillFieldType type);

  
  string16 label_;

  
  std::string guid_;

  
  std::vector<NameInfo> name_;
  std::vector<EmailInfo> email_;
  CompanyInfo company_;
  std::vector<HomePhoneNumber> home_number_;
  std::vector<FaxNumber> fax_number_;
  Address address_;
};

std::ostream& operator<<(std::ostream& os, const AutofillProfile& profile);

#endif  
