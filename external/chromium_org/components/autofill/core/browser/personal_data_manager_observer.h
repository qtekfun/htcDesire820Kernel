// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_PERSONAL_DATA_MANAGER_OBSERVER_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_PERSONAL_DATA_MANAGER_OBSERVER_H_

namespace autofill {

class PersonalDataManagerObserver {
 public:
  
  virtual void OnPersonalDataChanged() = 0;
  virtual void OnInsufficientFormData() {}

 protected:
  virtual ~PersonalDataManagerObserver() {}
};

}  

#endif  
