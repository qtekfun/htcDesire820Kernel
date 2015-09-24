// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_AUTOFILL_PROFILE_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_AUTOFILL_PROFILE_H_

#include <stddef.h>

#include <iosfwd>
#include <list>
#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/strings/string16.h"
#include "components/autofill/core/browser/address.h"
#include "components/autofill/core/browser/autofill_data_model.h"
#include "components/autofill/core/browser/autofill_type.h"
#include "components/autofill/core/browser/contact_info.h"
#include "components/autofill/core/browser/phone_number.h"

namespace autofill {

struct FormFieldData;

class AutofillProfile : public AutofillDataModel {
 public:
  AutofillProfile(const std::string& guid, const std::string& origin);

  
  AutofillProfile();
  AutofillProfile(const AutofillProfile& profile);
  virtual ~AutofillProfile();

  AutofillProfile& operator=(const AutofillProfile& profile);

  
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

  
  virtual base::string16 GetInfoForVariant(
      const AutofillType& type,
      size_t variant,
      const std::string& app_locale) const OVERRIDE;

  
  void SetRawMultiInfo(ServerFieldType type,
                       const std::vector<base::string16>& values);
  void GetRawMultiInfo(ServerFieldType type,
                       std::vector<base::string16>* values) const;
  void GetMultiInfo(const AutofillType& type,
                    const std::string& app_locale,
                    std::vector<base::string16>* values) const;

  
  bool IsEmpty(const std::string& app_locale) const;

  
  
  bool IsPresentButInvalid(ServerFieldType type) const;

  
  
  
  
  
  
  
  int Compare(const AutofillProfile& profile) const;

  
  
  bool operator==(const AutofillProfile& profile) const;
  virtual bool operator!=(const AutofillProfile& profile) const;

  
  
  
  const base::string16 PrimaryValue() const;

  
  
  bool IsSubsetOf(const AutofillProfile& profile,
                  const std::string& app_locale) const;

  
  
  void OverwriteWithOrAddTo(const AutofillProfile& profile,
                            const std::string& app_locale);

  
  static bool SupportsMultiValue(ServerFieldType type);

  
  
  
  
  
  
  
  static void CreateDifferentiatingLabels(
      const std::vector<AutofillProfile*>& profiles,
      std::vector<base::string16>* labels);

  
  
  
  
  
  
  
  static void CreateInferredLabels(
      const std::vector<AutofillProfile*>& profiles,
      const std::vector<ServerFieldType>* suggested_fields,
      ServerFieldType excluded_field,
      size_t minimal_fields_shown,
      std::vector<base::string16>* labels);

 private:
  typedef std::vector<const FormGroup*> FormGroupList;

  
  virtual void GetSupportedTypes(
      ServerFieldTypeSet* supported_types) const OVERRIDE;

  
  
  
  void GetMultiInfoImpl(const AutofillType& type,
                        const std::string& app_locale,
                        std::vector<base::string16>* values) const;

  
  
  
  
  void AddPhoneIfUnique(const base::string16& phone,
                        const std::string& app_locale,
                        std::vector<base::string16>* existing_phones);

  
  
  
  base::string16 ConstructInferredLabel(
      const std::vector<ServerFieldType>& label_fields,
      size_t num_fields_to_include) const;

  
  
  
  
  
  static void CreateInferredLabelsHelper(
      const std::vector<AutofillProfile*>& profiles,
      const std::list<size_t>& indices,
      const std::vector<ServerFieldType>& fields,
      size_t num_fields_to_include,
      std::vector<base::string16>* labels);

  
  
  FormGroupList FormGroups() const;
  const FormGroup* FormGroupForType(const AutofillType& type) const;
  FormGroup* MutableFormGroupForType(const AutofillType& type);

  
  std::vector<NameInfo> name_;
  std::vector<EmailInfo> email_;
  CompanyInfo company_;
  std::vector<PhoneNumber> phone_number_;
  Address address_;
};

std::ostream& operator<<(std::ostream& os, const AutofillProfile& profile);

}  

#endif  
