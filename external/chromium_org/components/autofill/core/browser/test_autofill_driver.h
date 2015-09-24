// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_TEST_AUTOFILL_DRIVER_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_TEST_AUTOFILL_DRIVER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "components/autofill/core/browser/autofill_driver.h"

namespace autofill {

class TestAutofillDriver : public AutofillDriver {
 public:
  TestAutofillDriver();
  virtual ~TestAutofillDriver();

  
  virtual bool IsOffTheRecord() const OVERRIDE;
  
  
  virtual net::URLRequestContextGetter* GetURLRequestContext() OVERRIDE;
  virtual base::SequencedWorkerPool* GetBlockingPool() OVERRIDE;
  virtual bool RendererIsAvailable() OVERRIDE;
  virtual void SetRendererActionOnFormDataReception(
      RendererFormDataAction action) OVERRIDE;
  virtual void SendFormDataToRenderer(int query_id,
                                      const FormData& data) OVERRIDE;
  virtual void SendAutofillTypePredictionsToRenderer(
      const std::vector<FormStructure*>& forms) OVERRIDE;
  virtual void RendererShouldAcceptDataListSuggestion(
      const base::string16& value) OVERRIDE;
  virtual void RendererShouldAcceptPasswordAutofillSuggestion(
      const base::string16& username) OVERRIDE;
  virtual void RendererShouldClearFilledForm() OVERRIDE;
  virtual void RendererShouldClearPreviewedForm() OVERRIDE;
  virtual void RendererShouldSetNodeText(const base::string16& value) OVERRIDE;

  

  
  
  void SetURLRequestContext(net::URLRequestContextGetter* url_request_context);

 private:
  scoped_refptr<base::SequencedWorkerPool> blocking_pool_;
  net::URLRequestContextGetter* url_request_context_;

  DISALLOW_COPY_AND_ASSIGN(TestAutofillDriver);
};

}  

#endif  
