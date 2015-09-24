// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_AUTOFILL_EXTERNAL_DELEGATE_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_AUTOFILL_EXTERNAL_DELEGATE_H_

#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "components/autofill/core/browser/autofill_popup_delegate.h"
#include "components/autofill/core/browser/password_autofill_manager.h"
#include "components/autofill/core/common/form_data.h"
#include "components/autofill/core/common/form_field_data.h"
#include "components/autofill/core/common/password_form_fill_data.h"
#include "ui/gfx/rect.h"

namespace gfx {
class Rect;
}

namespace autofill {

class AutofillDriver;
class AutofillManager;


class AutofillExternalDelegate
    : public AutofillPopupDelegate {
 public:
  
  
  AutofillExternalDelegate(AutofillManager* autofill_manager,
                           AutofillDriver* autofill_driver);
  virtual ~AutofillExternalDelegate();

  
  virtual void OnPopupShown() OVERRIDE;
  virtual void OnPopupHidden() OVERRIDE;
  virtual bool ShouldRepostEvent(const ui::MouseEvent& event) OVERRIDE;
  virtual void DidSelectSuggestion(int identifier) OVERRIDE;
  virtual void DidAcceptSuggestion(const base::string16& value,
                                   int identifier) OVERRIDE;
  virtual void RemoveSuggestion(const base::string16& value,
                                int identifier) OVERRIDE;
  virtual void ClearPreviewedForm() OVERRIDE;

  
  
  
  
  
  
  
  virtual void OnQuery(int query_id,
                       const FormData& form,
                       const FormFieldData& field,
                       const gfx::RectF& element_bounds,
                       bool display_warning_if_disabled);

  
  
  virtual void OnSuggestionsReturned(
      int query_id,
      const std::vector<base::string16>& autofill_values,
      const std::vector<base::string16>& autofill_labels,
      const std::vector<base::string16>& autofill_icons,
      const std::vector<int>& autofill_unique_ids);

  
  void OnShowPasswordSuggestions(const std::vector<base::string16>& suggestions,
                                 const std::vector<base::string16>& realms,
                                 const FormFieldData& field,
                                 const gfx::RectF& bounds);

  
  void SetCurrentDataListValues(
      const std::vector<base::string16>& data_list_values,
      const std::vector<base::string16>& data_list_labels);

  
  
  void DidEndTextFieldEditing();

  
  
  void Reset();

  
  void AddPasswordFormMapping(
      const FormFieldData& form,
      const PasswordFormFillData& fill_data);

 protected:
  base::WeakPtr<AutofillExternalDelegate> GetWeakPtr();

 private:
  
  
  
  
  void FillAutofillFormData(int unique_id, bool is_preview);

  
  void ApplyAutofillWarnings(std::vector<base::string16>* autofill_values,
                             std::vector<base::string16>* autofill_labels,
                             std::vector<base::string16>* autofill_icons,
                             std::vector<int>* autofill_unique_ids);

  
  
  
  void ApplyAutofillOptions(std::vector<base::string16>* autofill_values,
                            std::vector<base::string16>* autofill_labels,
                            std::vector<base::string16>* autofill_icons,
                            std::vector<int>* autofill_unique_ids);

  
  
  void InsertDataListValues(std::vector<base::string16>* autofill_values,
                            std::vector<base::string16>* autofill_labels,
                            std::vector<base::string16>* autofill_icons,
                            std::vector<int>* autofill_unique_ids);

  AutofillManager* autofill_manager_;  

  
  
  AutofillDriver* autofill_driver_;  

  
  PasswordAutofillManager password_autofill_manager_;

  
  
  int autofill_query_id_;

  
  FormData autofill_query_form_;
  FormFieldData autofill_query_field_;

  
  gfx::RectF element_bounds_;

  
  bool display_warning_if_disabled_;

  
  bool has_autofill_suggestion_;

  
  
  bool has_shown_autofill_popup_for_current_edit_;

  
  std::vector<base::string16> data_list_values_;
  std::vector<base::string16> data_list_labels_;

  base::WeakPtrFactory<AutofillExternalDelegate> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(AutofillExternalDelegate);
};

}  

#endif  
