// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_WEBDATA_AUTOFILL_WEBDATA_SERVICE_OBSERVER_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_WEBDATA_AUTOFILL_WEBDATA_SERVICE_OBSERVER_H_

#include "components/autofill/core/browser/webdata/autofill_change.h"

namespace autofill {

class AutofillWebDataServiceObserverOnDBThread {
 public:
  
  virtual void AutofillEntriesChanged(const AutofillChangeList& changes) {}

  
  
  virtual void AutofillProfileChanged(const AutofillProfileChange& change) {}

 protected:
  virtual ~AutofillWebDataServiceObserverOnDBThread() {}
};

class AutofillWebDataServiceObserverOnUIThread {
 public:
  
  
  virtual void AutofillMultipleChanged() {}

 protected:
  virtual ~AutofillWebDataServiceObserverOnUIThread() {}
};

}  

#endif  
