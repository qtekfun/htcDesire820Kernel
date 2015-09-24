// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CONTENT_RENDERER_PASSWORD_GENERATION_AGENT_H_
#define COMPONENTS_AUTOFILL_CONTENT_RENDERER_PASSWORD_GENERATION_AGENT_H_

#include <map>
#include <utility>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "content/public/renderer/render_view_observer.h"
#include "third_party/WebKit/public/web/WebInputElement.h"
#include "third_party/WebKit/public/web/WebPasswordGeneratorClient.h"
#include "url/gurl.h"

namespace blink {
class WebCString;
class WebDocument;
}

namespace autofill {

struct FormData;
struct PasswordForm;

class PasswordGenerationAgent : public content::RenderViewObserver,
                                public blink::WebPasswordGeneratorClient {
 public:
  explicit PasswordGenerationAgent(content::RenderView* render_view);
  virtual ~PasswordGenerationAgent();

 protected:
  
  
  virtual bool ShouldAnalyzeDocument(const blink::WebDocument& document) const;

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

 private:
  
  virtual void DidFinishDocumentLoad(blink::WebFrame* frame) OVERRIDE;
  virtual void DidFinishLoad(blink::WebFrame* frame) OVERRIDE;

  
  virtual void openPasswordGenerator(blink::WebInputElement& element) OVERRIDE;

  
  void OnFormNotBlacklisted(const PasswordForm& form);
  void OnPasswordAccepted(const base::string16& password);
  void OnAccountCreationFormsDetected(
      const std::vector<autofill::FormData>& forms);

  
  void MaybeShowIcon();

  content::RenderView* render_view_;

  
  scoped_ptr<PasswordForm> possible_account_creation_form_;

  
  
  
  std::vector<GURL> not_blacklisted_password_form_origins_;

  
  
  
  std::vector<autofill::FormData> generation_enabled_forms_;

  std::vector<blink::WebInputElement> passwords_;

  DISALLOW_COPY_AND_ASSIGN(PasswordGenerationAgent);
};

}  

#endif  
