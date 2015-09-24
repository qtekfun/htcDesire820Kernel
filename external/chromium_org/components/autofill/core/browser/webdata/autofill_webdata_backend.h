// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_WEBDATA_AUTOFILL_WEBDATA_BACKEND_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_WEBDATA_AUTOFILL_WEBDATA_BACKEND_H_

class WebDatabase;

namespace autofill {

class AutofillWebDataServiceObserverOnDBThread;

class AutofillWebDataBackend {
 public:
  virtual ~AutofillWebDataBackend() {}

  
  virtual WebDatabase* GetDatabase() = 0;

  
  virtual void AddObserver(
      AutofillWebDataServiceObserverOnDBThread* observer) = 0;

  
  virtual void RemoveObserver(
      AutofillWebDataServiceObserverOnDBThread* observer) = 0;

  
  virtual void RemoveExpiredFormElements() = 0;

  
  
  
  
  virtual void NotifyOfMultipleAutofillChanges() = 0;
};

} 

#endif  
