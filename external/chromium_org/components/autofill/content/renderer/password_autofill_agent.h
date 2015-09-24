// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CONTENT_RENDERER_PASSWORD_AUTOFILL_AGENT_H_
#define COMPONENTS_AUTOFILL_CONTENT_RENDERER_PASSWORD_AUTOFILL_AGENT_H_

#include <map>
#include <vector>

#include "base/memory/linked_ptr.h"
#include "base/memory/weak_ptr.h"
#include "components/autofill/core/common/password_form_fill_data.h"
#include "content/public/renderer/render_view_observer.h"
#include "third_party/WebKit/public/web/WebInputElement.h"

namespace blink {
class WebInputElement;
class WebKeyboardEvent;
class WebSecurityOrigin;
class WebView;
}

namespace autofill {

class PasswordAutofillAgent : public content::RenderViewObserver {
 public:
  explicit PasswordAutofillAgent(content::RenderView* render_view);
  virtual ~PasswordAutofillAgent();

  
  
  
  bool TextFieldDidEndEditing(const blink::WebInputElement& element);
  bool TextDidChangeInTextField(const blink::WebInputElement& element);
  bool TextFieldHandlingKeyDown(const blink::WebInputElement& element,
                                const blink::WebKeyboardEvent& event);

  
  
  bool DidAcceptAutofillSuggestion(const blink::WebNode& node,
                                   const blink::WebString& username);
  
  
  
  bool DidClearAutofillSelection(const blink::WebNode& node);
  
  
  bool ShowSuggestions(const blink::WebInputElement& element);

  
  void OnDynamicFormsSeen(blink::WebFrame* frame);

 protected:
  virtual bool OriginCanAccessPasswordManager(
      const blink::WebSecurityOrigin& origin);

 private:
  friend class PasswordAutofillAgentTest;

  enum OtherPossibleUsernamesUsage {
    NOTHING_TO_AUTOFILL,
    OTHER_POSSIBLE_USERNAMES_ABSENT,
    OTHER_POSSIBLE_USERNAMES_PRESENT,
    OTHER_POSSIBLE_USERNAME_SHOWN,
    OTHER_POSSIBLE_USERNAME_SELECTED,
    OTHER_POSSIBLE_USERNAMES_MAX
  };

  struct PasswordInfo {
    blink::WebInputElement password_field;
    PasswordFormFillData fill_data;
    bool backspace_pressed_last;
    PasswordInfo() : backspace_pressed_last(false) {}
  };
  typedef std::map<blink::WebElement, PasswordInfo> LoginToPasswordInfoMap;
  typedef std::map<blink::WebFrame*,
                   linked_ptr<PasswordForm> > FrameToPasswordFormMap;

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void DidStartProvisionalLoad(blink::WebFrame* frame) OVERRIDE;
  virtual void DidStartLoading() OVERRIDE;
  virtual void DidFinishDocumentLoad(blink::WebFrame* frame) OVERRIDE;
  virtual void DidFinishLoad(blink::WebFrame* frame) OVERRIDE;
  virtual void FrameDetached(blink::WebFrame* frame) OVERRIDE;
  virtual void FrameWillClose(blink::WebFrame* frame) OVERRIDE;
  virtual void WillSendSubmitEvent(blink::WebFrame* frame,
                                   const blink::WebFormElement& form) OVERRIDE;
  virtual void WillSubmitForm(blink::WebFrame* frame,
                              const blink::WebFormElement& form) OVERRIDE;

  
  void OnFillPasswordForm(const PasswordFormFillData& form_data);

  
  
  void SendPasswordForms(blink::WebFrame* frame, bool only_visible);

  void GetSuggestions(const PasswordFormFillData& fill_data,
                      const base::string16& input,
                      std::vector<base::string16>* suggestions,
                      std::vector<base::string16>* realms);

  bool ShowSuggestionPopup(const PasswordFormFillData& fill_data,
                           const blink::WebInputElement& user_input);

  
  
  
  
  
  void FillFormOnPasswordRecieved(const PasswordFormFillData& fill_data,
                                  blink::WebInputElement username_element,
                                  blink::WebInputElement password_element);

  bool FillUserNameAndPassword(
      blink::WebInputElement* username_element,
      blink::WebInputElement* password_element,
      const PasswordFormFillData& fill_data,
      bool exact_username_match,
      bool set_selection);

  
  
  void PerformInlineAutocomplete(
      const blink::WebInputElement& username,
      const blink::WebInputElement& password,
      const PasswordFormFillData& fill_data);

  
  
  void FrameClosing(const blink::WebFrame* frame);

  
  bool FindLoginInfo(const blink::WebNode& node,
                     blink::WebInputElement* found_input,
                     PasswordInfo* found_password);

  
  
  blink::WebFrame* CurrentOrChildFrameWithSavedForms(
      const blink::WebFrame* current_frame);

  
  LoginToPasswordInfoMap login_to_password_info_;

  
  OtherPossibleUsernamesUsage usernames_usage_;

  
  blink::WebView* web_view_;

  
  
  FrameToPasswordFormMap provisionally_saved_forms_;

  base::WeakPtrFactory<PasswordAutofillAgent> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(PasswordAutofillAgent);
};

}  

#endif  
