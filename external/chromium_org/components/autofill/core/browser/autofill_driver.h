// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_AUTOFILL_DRIVER_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_AUTOFILL_DRIVER_H_

#include <vector>

#include "components/autofill/core/common/form_data.h"

namespace base {
class SequencedWorkerPool;
}

namespace net {
class URLRequestContextGetter;
}

namespace autofill {

class FormStructure;

class AutofillDriver {
 public:
   
  enum RendererFormDataAction {
    
    FORM_DATA_ACTION_FILL,
    
    FORM_DATA_ACTION_PREVIEW
  };

  virtual ~AutofillDriver() {}

  
  
  virtual bool IsOffTheRecord() const = 0;

  
  virtual net::URLRequestContextGetter* GetURLRequestContext() = 0;

  
  
  virtual base::SequencedWorkerPool* GetBlockingPool() = 0;

  
  virtual bool RendererIsAvailable() = 0;

  
  
  virtual void SetRendererActionOnFormDataReception(
      RendererFormDataAction action) = 0;

  
  
  
  virtual void SendFormDataToRenderer(int query_id, const FormData& data) = 0;

  
  
  
  virtual void SendAutofillTypePredictionsToRenderer(
      const std::vector<FormStructure*>& forms) = 0;

  
  virtual void RendererShouldAcceptDataListSuggestion(
      const base::string16& value) = 0;

  
  
  virtual void RendererShouldAcceptPasswordAutofillSuggestion(
      const base::string16& username) = 0;

  
  virtual void RendererShouldClearFilledForm() = 0;

  
  virtual void RendererShouldClearPreviewedForm() = 0;

  
  virtual void RendererShouldSetNodeText(const base::string16& value) = 0;
};

}  

#endif  
