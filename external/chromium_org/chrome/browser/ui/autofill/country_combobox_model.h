// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_AUTOFILL_COUNTRY_COMBOBOX_MODEL_H_
#define CHROME_BROWSER_UI_AUTOFILL_COUNTRY_COMBOBOX_MODEL_H_

#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/scoped_vector.h"
#include "base/strings/string16.h"
#include "ui/base/models/combobox_model.h"

namespace autofill {

class AutofillCountry;
class PersonalDataManager;

class CountryComboboxModel : public ui::ComboboxModel {
 public:
  explicit CountryComboboxModel(const PersonalDataManager& manager);
  virtual ~CountryComboboxModel();

  
  virtual int GetItemCount() const OVERRIDE;
  virtual base::string16 GetItemAt(int index) OVERRIDE;
  virtual bool IsItemSeparatorAt(int index) OVERRIDE;

  const std::vector<AutofillCountry*>& countries() const {
    return countries_.get();
  }

 private:
  
  
  ScopedVector<AutofillCountry> countries_;

  DISALLOW_COPY_AND_ASSIGN(CountryComboboxModel);
};

}  

#endif  