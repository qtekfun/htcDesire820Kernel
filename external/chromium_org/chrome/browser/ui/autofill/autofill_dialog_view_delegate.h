// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_AUTOFILL_AUTOFILL_DIALOG_VIEW_DELEGATE_H_
#define CHROME_BROWSER_UI_AUTOFILL_AUTOFILL_DIALOG_VIEW_DELEGATE_H_

#include <vector>

#include "base/strings/string16.h"
#include "chrome/browser/ui/autofill/autofill_dialog_types.h"
#include "components/autofill/content/browser/wallet/required_action.h"
#include "components/autofill/core/browser/field_types.h"
#include "ui/base/ui_base_types.h"
#include "ui/gfx/image/image.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/range/range.h"

class FieldValueMap;
class GURL;
class Profile;

namespace content {
class WebContents;
struct NativeWebKeyboardEvent;
}

namespace gfx {
class Rect;
}

namespace ui {
class ComboboxModel;
class MenuModel;
}

namespace autofill {

typedef std::map<ServerFieldType, gfx::Image> FieldIconMap;

class AutofillDialogViewDelegate {
 public:
  

  virtual base::string16 DialogTitle() const = 0;
  virtual base::string16 AccountChooserText() const = 0;
  virtual base::string16 SignInLinkText() const = 0;
  virtual base::string16 SpinnerText() const = 0;
  virtual base::string16 EditSuggestionText() const = 0;
  virtual base::string16 CancelButtonText() const = 0;
  virtual base::string16 ConfirmButtonText() const = 0;
  virtual base::string16 SaveLocallyText() const = 0;
  virtual base::string16 SaveLocallyTooltip() const = 0;
  virtual base::string16 LegalDocumentsText() = 0;

  

  
  
  virtual bool ShouldShowSpinner() const = 0;

  
  virtual bool ShouldShowAccountChooser() const = 0;

  
  virtual bool ShouldShowSignInWebView() const = 0;

  
  virtual GURL SignInUrl() const = 0;

  
  virtual bool ShouldOfferToSaveInChrome() const = 0;

  
  virtual bool ShouldSaveInChrome() const = 0;

  
  
  
  virtual ui::MenuModel* MenuModelForAccountChooser() = 0;

  
  virtual gfx::Image AccountChooserImage() = 0;

  
  
  virtual gfx::Image ButtonStripImage() const = 0;

  
  virtual int GetDialogButtons() const = 0;

  
  virtual bool IsDialogButtonEnabled(ui::DialogButton button) const = 0;

  
  
  virtual DialogOverlayState GetDialogOverlay() = 0;

  
  virtual const std::vector<gfx::Range>& LegalDocumentLinks() = 0;

  

  
  virtual bool SectionIsActive(DialogSection section) const = 0;

  
  
  virtual const DetailInputs& RequestedFieldsForSection(DialogSection section)
      const = 0;

  
  
  virtual ui::ComboboxModel* ComboboxModelForAutofillType(
      ServerFieldType type) = 0;

  
  
  
  virtual ui::MenuModel* MenuModelForSection(DialogSection section) = 0;

  
  virtual base::string16 LabelForSection(DialogSection section) const = 0;

  
  virtual SuggestionState SuggestionStateForSection(DialogSection section) = 0;

  
  
  virtual FieldIconMap IconsForFields(
      const FieldValueMap& user_inputs) const = 0;

  
  
  virtual bool FieldControlsIcons(ServerFieldType type) const = 0;

  
  virtual base::string16 TooltipForField(ServerFieldType type) const = 0;

  
  virtual bool InputIsEditable(const DetailInput& input,
                               DialogSection section) = 0;

  
  
  
  virtual base::string16 InputValidityMessage(DialogSection section,
                                        ServerFieldType type,
                                        const base::string16& value) = 0;


  
  
  virtual ValidityMessages InputsAreValid(DialogSection section,
                                          const FieldValueMap& inputs) = 0;

  
  
  
  virtual void UserEditedOrActivatedInput(DialogSection section,
                                          ServerFieldType type,
                                          gfx::NativeView parent_view,
                                          const gfx::Rect& content_bounds,
                                          const base::string16& field_contents,
                                          bool was_edit) = 0;

  
  
  virtual bool HandleKeyPressEventInInput(
      const content::NativeWebKeyboardEvent& event) = 0;

  
  virtual void FocusMoved() = 0;

  
  virtual bool ShouldShowErrorBubble() const = 0;

  

  
  virtual void ViewClosed() = 0;

  
  
  virtual std::vector<DialogNotification> CurrentNotifications() = 0;

  
  
  virtual void LinkClicked(const GURL& url) = 0;

  
  virtual void SignInLinkClicked() = 0;

  
  virtual void NotificationCheckboxStateChanged(DialogNotification::Type type,
                                                bool checked) = 0;

  
  virtual void LegalDocumentLinkClicked(const gfx::Range& range) = 0;

  
  
  virtual bool OnCancel() = 0;

  
  
  
  virtual bool OnAccept() = 0;

  
  virtual Profile* profile() = 0;

  
  virtual content::WebContents* GetWebContents() = 0;

 protected:
  virtual ~AutofillDialogViewDelegate();
};

}  

#endif  
