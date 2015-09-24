// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CONTENT_BROWSER_AUTOFILL_DRIVER_IMPL_H_
#define COMPONENTS_AUTOFILL_CONTENT_BROWSER_AUTOFILL_DRIVER_IMPL_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/supports_user_data.h"
#include "components/autofill/content/browser/request_autocomplete_manager.h"
#include "components/autofill/core/browser/autofill_driver.h"
#include "components/autofill/core/browser/autofill_external_delegate.h"
#include "components/autofill/core/browser/autofill_manager.h"
#include "content/public/browser/web_contents_observer.h"

namespace content {
class WebContents;
}

namespace IPC {
class Message;
}

namespace autofill {

class AutofillContext;
class AutofillManagerDelegate;

class AutofillDriverImpl : public AutofillDriver,
                           public content::WebContentsObserver,
                           public base::SupportsUserData::Data {
 public:
  static void CreateForWebContentsAndDelegate(
      content::WebContents* contents,
      autofill::AutofillManagerDelegate* delegate,
      const std::string& app_locale,
      AutofillManager::AutofillDownloadManagerState enable_download_manager);
  static AutofillDriverImpl* FromWebContents(content::WebContents* contents);

  
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

  
  content::WebContents* GetWebContents();

  AutofillExternalDelegate* autofill_external_delegate() {
    return &autofill_external_delegate_;
  }

  AutofillManager* autofill_manager() { return autofill_manager_.get(); }

 protected:
  AutofillDriverImpl(
      content::WebContents* web_contents,
      autofill::AutofillManagerDelegate* delegate,
      const std::string& app_locale,
      AutofillManager::AutofillDownloadManagerState enable_download_manager);
  virtual ~AutofillDriverImpl();

  
  virtual void DidNavigateMainFrame(
      const content::LoadCommittedDetails& details,
      const content::FrameNavigateParams& params) OVERRIDE;
  virtual void NavigationEntryCommitted(
      const content::LoadCommittedDetails& load_details) OVERRIDE;
  virtual void WasHidden() OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  
  
  void SetAutofillManager(scoped_ptr<AutofillManager> manager);

 private:
  
  
  scoped_ptr<AutofillManager> autofill_manager_;

  
  
  AutofillExternalDelegate autofill_external_delegate_;

  
  RequestAutocompleteManager request_autocomplete_manager_;
};

}  

#endif  
