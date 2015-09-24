// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_PHONE_NUMBER_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_PHONE_NUMBER_H_

#include <string>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/strings/string16.h"
#include "components/autofill/core/browser/form_group.h"
#include "components/autofill/core/browser/phone_number_i18n.h"

namespace autofill {

class AutofillProfile;

class PhoneNumber : public FormGroup {
 public:
  explicit PhoneNumber(AutofillProfile* profile);
  PhoneNumber(const PhoneNumber& number);
  virtual ~PhoneNumber();

  PhoneNumber& operator=(const PhoneNumber& number);

  void set_profile(AutofillProfile* profile) { profile_ = profile; }

  
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

  
  static const size_t kPrefixOffset = 0;
  static const size_t kPrefixLength = 3;
  static const size_t kSuffixOffset = 3;
  static const size_t kSuffixLength = 4;

  
  class PhoneCombineHelper {
   public:
    PhoneCombineHelper();
    ~PhoneCombineHelper();

    
    
    bool SetInfo(const AutofillType& type, const base::string16& value);

    
    
    
    
    bool ParseNumber(const AutofillProfile& profile,
                     const std::string& app_locale,
                     base::string16* value);

    
    bool IsEmpty() const;

   private:
    base::string16 country_;
    base::string16 city_;
    base::string16 phone_;
    base::string16 whole_number_;
  };

 private:
  
  virtual void GetSupportedTypes(
      ServerFieldTypeSet* supported_types) const OVERRIDE;

  
  
  void UpdateCacheIfNeeded(const std::string& app_locale) const;

  
  base::string16 number_;
  
  const AutofillProfile* profile_;  

  
  mutable i18n::PhoneObject cached_parsed_phone_;
};

}  

#endif  
