// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_AUTOFILL_MANAGER_TEST_DELEGATE_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_AUTOFILL_MANAGER_TEST_DELEGATE_H_

namespace autofill {

class AutofillManagerTestDelegate {
 public:
  virtual ~AutofillManagerTestDelegate() {}

  
  virtual void DidPreviewFormData() = 0;

  
  virtual void DidFillFormData() = 0;

  
  virtual void DidShowSuggestions() = 0;
};

}  

#endif  
