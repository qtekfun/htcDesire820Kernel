// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CONTENT_RENDERER_AUTOFILL_AGENT_H_
#define COMPONENTS_AUTOFILL_CONTENT_RENDERER_AUTOFILL_AGENT_H_

#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "components/autofill/content/renderer/form_cache.h"
#include "components/autofill/content/renderer/page_click_listener.h"
#include "components/autofill/core/common/forms_seen_state.h"
#include "content/public/renderer/render_view_observer.h"
#include "third_party/WebKit/public/web/WebAutofillClient.h"
#include "third_party/WebKit/public/web/WebFormElement.h"
#include "third_party/WebKit/public/web/WebInputElement.h"

namespace blink {
class WebNode;
class WebView;
}

namespace autofill {

struct FormData;
struct FormFieldData;
struct WebElementDescriptor;
class PasswordAutofillAgent;


class AutofillAgent : public content::RenderViewObserver,
                      public PageClickListener,
                      public blink::WebAutofillClient {
 public:
  
  AutofillAgent(content::RenderView* render_view,
                PasswordAutofillAgent* password_autofill_manager);
  virtual ~AutofillAgent();

 private:
  enum AutofillAction {
    AUTOFILL_NONE,     
    AUTOFILL_FILL,     
    AUTOFILL_PREVIEW,  
  };

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void DidFinishDocumentLoad(blink::WebFrame* frame) OVERRIDE;
  virtual void DidCommitProvisionalLoad(blink::WebFrame* frame,
                                        bool is_new_navigation) OVERRIDE;
  virtual void FrameDetached(blink::WebFrame* frame) OVERRIDE;
  virtual void WillSubmitForm(blink::WebFrame* frame,
                              const blink::WebFormElement& form) OVERRIDE;
  virtual void ZoomLevelChanged() OVERRIDE;
  virtual void DidChangeScrollOffset(blink::WebFrame* frame) OVERRIDE;
  virtual void FocusedNodeChanged(const blink::WebNode& node) OVERRIDE;
  virtual void OrientationChangeEvent(int orientation) OVERRIDE;

  
  virtual void InputElementClicked(const blink::WebInputElement& element,
                                   bool was_focused,
                                   bool is_focused) OVERRIDE;
  virtual void InputElementLostFocus() OVERRIDE;

  
  virtual void didClearAutofillSelection(const blink::WebNode& node) OVERRIDE;
  virtual void textFieldDidEndEditing(
      const blink::WebInputElement& element) OVERRIDE;
  virtual void textFieldDidChange(
      const blink::WebInputElement& element) OVERRIDE;
  virtual void textFieldDidReceiveKeyDown(
      const blink::WebInputElement& element,
      const blink::WebKeyboardEvent& event) OVERRIDE;
  virtual void didRequestAutocomplete(
      blink::WebFrame* frame,
      const blink::WebFormElement& form) OVERRIDE;
  virtual void setIgnoreTextChanges(bool ignore) OVERRIDE;
  virtual void didAssociateFormControls(
      const blink::WebVector<blink::WebNode>& nodes) OVERRIDE;
  virtual void openTextDataListChooser(const blink::WebInputElement& element);

  void OnFormDataFilled(int query_id, const FormData& form);
  void OnFieldTypePredictionsAvailable(
      const std::vector<FormDataPredictions>& forms);

  
  void OnSetAutofillActionFill();
  void OnClearForm();
  void OnSetAutofillActionPreview();
  void OnClearPreviewedForm();
  void OnSetNodeText(const base::string16& value);
  void OnAcceptDataListSuggestion(const base::string16& value);
  void OnAcceptPasswordAutofillSuggestion(const base::string16& username);

  
  void OnRequestAutocompleteResult(
      blink::WebFormElement::AutocompleteResult result,
      const FormData& form_data);

  
  void FinishAutocompleteRequest(
      blink::WebFormElement::AutocompleteResult result);

  
  
  void OnPageShown();

  
  
  void TextFieldDidChangeImpl(const blink::WebInputElement& element);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void ShowSuggestions(const blink::WebInputElement& element,
                       bool autofill_on_empty_values,
                       bool requires_caret_at_end,
                       bool display_warning_if_disabled,
                       bool datalist_only);

  
  
  void QueryAutofillSuggestions(const blink::WebInputElement& element,
                                bool display_warning_if_disabled,
                                bool datalist_only);

  
  void AcceptDataListSuggestion(const base::string16& suggested_value);

  
  
  
  
  void FillAutofillFormData(const blink::WebNode& node,
                            int unique_id,
                            AutofillAction action);

  
  
  bool FindFormAndFieldForNode(
      const blink::WebNode& node,
      FormData* form,
      FormFieldData* field) WARN_UNUSED_RESULT;

  
  void SetNodeText(const base::string16& value, blink::WebInputElement* node);

  
  void HideAutofillUI();

  FormCache form_cache_;

  PasswordAutofillAgent* password_autofill_agent_;  

  
  
  int autofill_query_id_;

  
  blink::WebInputElement element_;

  
  blink::WebFormElement in_flight_request_form_;

  
  std::vector<blink::WebFormElement> form_elements_;

  
  AutofillAction autofill_action_;

  
  blink::WebView* web_view_;

  
  bool display_warning_if_disabled_;

  
  bool was_query_node_autofilled_;

  
  
  bool has_shown_autofill_popup_for_current_edit_;

  
  bool did_set_node_text_;

  
  
  bool has_new_forms_for_browser_;

  
  
  
  bool ignore_text_changes_;

  
  base::TimeTicks forms_seen_timestamp_;

  base::WeakPtrFactory<AutofillAgent> weak_ptr_factory_;

  friend class PasswordAutofillAgentTest;
  FRIEND_TEST_ALL_PREFIXES(ChromeRenderViewTest, FillFormElement);
  FRIEND_TEST_ALL_PREFIXES(ChromeRenderViewTest, SendForms);
  FRIEND_TEST_ALL_PREFIXES(ChromeRenderViewTest, SendDynamicForms);
  FRIEND_TEST_ALL_PREFIXES(ChromeRenderViewTest, ShowAutofillWarning);
  FRIEND_TEST_ALL_PREFIXES(PasswordAutofillAgentTest, WaitUsername);
  FRIEND_TEST_ALL_PREFIXES(PasswordAutofillAgentTest, SuggestionAccept);
  FRIEND_TEST_ALL_PREFIXES(PasswordAutofillAgentTest, SuggestionSelect);

  DISALLOW_COPY_AND_ASSIGN(AutofillAgent);
};

}  

#endif  
