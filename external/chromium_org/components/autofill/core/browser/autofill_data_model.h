// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_AUTOFILL_DATA_MODEL_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_AUTOFILL_DATA_MODEL_H_

#include <string>

#include "base/strings/string16.h"
#include "components/autofill/core/browser/form_group.h"

namespace autofill {

class AutofillType;

class AutofillDataModel : public FormGroup {
 public:
  AutofillDataModel(const std::string& guid, const std::string& origin);
  virtual ~AutofillDataModel();

  
  
  
  
  virtual base::string16 GetInfoForVariant(const AutofillType& type,
                                           size_t variant,
                                           const std::string& app_locale) const;

  
  
  bool IsVerified() const;

  std::string guid() const { return guid_; }
  void set_guid(const std::string& guid) { guid_ = guid; }

  std::string origin() const { return origin_; }
  void set_origin(const std::string& origin) { origin_ = origin; }

 private:
  
  std::string guid_;

  
  
  
  
  
  
  std::string origin_;
};

}  

#endif  
