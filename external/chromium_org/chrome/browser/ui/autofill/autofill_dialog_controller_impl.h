// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_AUTOFILL_AUTOFILL_DIALOG_CONTROLLER_IMPL_H_
#define CHROME_BROWSER_UI_AUTOFILL_AUTOFILL_DIALOG_CONTROLLER_IMPL_H_

#include <set>
#include <vector>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "base/time/time.h"
#include "chrome/browser/ui/autofill/account_chooser_model.h"
#include "chrome/browser/ui/autofill/autofill_dialog_controller.h"
#include "chrome/browser/ui/autofill/autofill_dialog_models.h"
#include "chrome/browser/ui/autofill/autofill_dialog_types.h"
#include "chrome/browser/ui/autofill/autofill_dialog_view_delegate.h"
#include "chrome/browser/ui/autofill/autofill_popup_controller_impl.h"
#include "chrome/browser/ui/autofill/country_combobox_model.h"
#include "components/autofill/content/browser/wallet/wallet_client.h"
#include "components/autofill/content/browser/wallet/wallet_client_delegate.h"
#include "components/autofill/content/browser/wallet/wallet_items.h"
#include "components/autofill/content/browser/wallet/wallet_signin_helper_delegate.h"
#include "components/autofill/core/browser/autofill_manager_delegate.h"
#include "components/autofill/core/browser/autofill_metrics.h"
#include "components/autofill/core/browser/autofill_popup_delegate.h"
#include "components/autofill/core/browser/field_types.h"
#include "components/autofill/core/browser/form_structure.h"
#include "components/autofill/core/browser/personal_data_manager.h"
#include "components/autofill/core/browser/personal_data_manager_observer.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/common/ssl_status.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/base/models/simple_menu_model.h"
#include "ui/base/ui_base_types.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/gfx/animation/linear_animation.h"
#include "url/gurl.h"

class Profile;

namespace content {
class WebContents;
}

namespace autofill {

class AutofillDataModel;
class AutofillDialogView;
class AutofillPopupControllerImpl;
class DataModelWrapper;
class TestableAutofillDialogView;

namespace risk {
class Fingerprint;
}

namespace wallet {
class WalletSigninHelper;
}

class AutofillDialogControllerImpl : public AutofillDialogViewDelegate,
                                     public AutofillDialogController,
                                     public AutofillPopupDelegate,
                                     public content::NotificationObserver,
                                     public content::WebContentsObserver,
                                     public SuggestionsMenuModelDelegate,
                                     public wallet::WalletClientDelegate,
                                     public wallet::WalletSigninHelperDelegate,
                                     public PersonalDataManagerObserver,
                                     public AccountChooserModelDelegate,
                                     public gfx::AnimationDelegate {
 public:
  virtual ~AutofillDialogControllerImpl();

  static base::WeakPtr<AutofillDialogControllerImpl> Create(
      content::WebContents* contents,
      const FormData& form_structure,
      const GURL& source_url,
      const base::Callback<void(const FormStructure*)>& callback);

  
  virtual void Show() OVERRIDE;
  virtual void Hide() OVERRIDE;
  virtual void TabActivated() OVERRIDE;

  
  
  TestableAutofillDialogView* GetTestableView();

  
  virtual base::string16 DialogTitle() const OVERRIDE;
  virtual base::string16 AccountChooserText() const OVERRIDE;
  virtual base::string16 SignInLinkText() const OVERRIDE;
  virtual base::string16 SpinnerText() const OVERRIDE;
  virtual base::string16 EditSuggestionText() const OVERRIDE;
  virtual base::string16 CancelButtonText() const OVERRIDE;
  virtual base::string16 ConfirmButtonText() const OVERRIDE;
  virtual base::string16 SaveLocallyText() const OVERRIDE;
  virtual base::string16 SaveLocallyTooltip() const OVERRIDE;
  virtual base::string16 LegalDocumentsText() OVERRIDE;
  virtual bool ShouldShowSpinner() const OVERRIDE;
  virtual bool ShouldShowAccountChooser() const OVERRIDE;
  virtual bool ShouldShowSignInWebView() const OVERRIDE;
  virtual GURL SignInUrl() const OVERRIDE;
  virtual bool ShouldOfferToSaveInChrome() const OVERRIDE;
  virtual bool ShouldSaveInChrome() const OVERRIDE;
  virtual ui::MenuModel* MenuModelForAccountChooser() OVERRIDE;
  virtual gfx::Image AccountChooserImage() OVERRIDE;
  virtual gfx::Image ButtonStripImage() const OVERRIDE;
  virtual int GetDialogButtons() const OVERRIDE;
  virtual bool IsDialogButtonEnabled(ui::DialogButton button) const OVERRIDE;
  virtual DialogOverlayState GetDialogOverlay() OVERRIDE;
  virtual const std::vector<gfx::Range>& LegalDocumentLinks() OVERRIDE;
  virtual bool SectionIsActive(DialogSection section) const OVERRIDE;
  virtual const DetailInputs& RequestedFieldsForSection(DialogSection section)
      const OVERRIDE;
  virtual ui::ComboboxModel* ComboboxModelForAutofillType(
      ServerFieldType type) OVERRIDE;
  virtual ui::MenuModel* MenuModelForSection(DialogSection section) OVERRIDE;
  virtual base::string16 LabelForSection(DialogSection section) const OVERRIDE;
  virtual SuggestionState SuggestionStateForSection(
      DialogSection section) OVERRIDE;
  virtual FieldIconMap IconsForFields(const FieldValueMap& user_inputs)
      const OVERRIDE;
  virtual bool FieldControlsIcons(ServerFieldType type) const OVERRIDE;
  virtual base::string16 TooltipForField(ServerFieldType type) const OVERRIDE;
  virtual bool InputIsEditable(const DetailInput& input, DialogSection section)
      OVERRIDE;
  virtual base::string16 InputValidityMessage(DialogSection section,
                                        ServerFieldType type,
                                        const base::string16& value) OVERRIDE;
  virtual ValidityMessages InputsAreValid(
      DialogSection section, const FieldValueMap& inputs) OVERRIDE;
  virtual void UserEditedOrActivatedInput(DialogSection section,
                                          ServerFieldType type,
                                          gfx::NativeView parent_view,
                                          const gfx::Rect& content_bounds,
                                          const base::string16& field_contents,
                                          bool was_edit) OVERRIDE;
  virtual bool HandleKeyPressEventInInput(
      const content::NativeWebKeyboardEvent& event) OVERRIDE;
  virtual void FocusMoved() OVERRIDE;
  virtual bool ShouldShowErrorBubble() const OVERRIDE;
  virtual void ViewClosed() OVERRIDE;
  virtual std::vector<DialogNotification> CurrentNotifications() OVERRIDE;
  virtual void LinkClicked(const GURL& url) OVERRIDE;
  virtual void SignInLinkClicked() OVERRIDE;
  virtual void NotificationCheckboxStateChanged(DialogNotification::Type type,
                                                bool checked) OVERRIDE;
  virtual void LegalDocumentLinkClicked(const gfx::Range& range) OVERRIDE;
  virtual bool OnCancel() OVERRIDE;
  virtual bool OnAccept() OVERRIDE;
  virtual Profile* profile() OVERRIDE;
  virtual content::WebContents* GetWebContents() OVERRIDE;

  
  virtual void OnPopupShown() OVERRIDE;
  virtual void OnPopupHidden() OVERRIDE;
  virtual bool ShouldRepostEvent(const ui::MouseEvent& event) OVERRIDE;
  virtual void DidSelectSuggestion(int identifier) OVERRIDE;
  virtual void DidAcceptSuggestion(const base::string16& value,
                                   int identifier) OVERRIDE;
  virtual void RemoveSuggestion(const base::string16& value,
                                int identifier) OVERRIDE;
  virtual void ClearPreviewedForm() OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void SuggestionsMenuWillShow() OVERRIDE;
  virtual void SuggestionItemSelected(SuggestionsMenuModel* model,
                                      size_t index) OVERRIDE;

  
  virtual const AutofillMetrics& GetMetricLogger() const OVERRIDE;
  virtual std::string GetRiskData() const OVERRIDE;
  virtual std::string GetWalletCookieValue() const OVERRIDE;
  virtual bool IsShippingAddressRequired() const OVERRIDE;
  virtual void OnDidAcceptLegalDocuments() OVERRIDE;
  virtual void OnDidAuthenticateInstrument(bool success) OVERRIDE;
  virtual void OnDidGetFullWallet(
      scoped_ptr<wallet::FullWallet> full_wallet) OVERRIDE;
  virtual void OnDidGetWalletItems(
      scoped_ptr<wallet::WalletItems> wallet_items) OVERRIDE;
  virtual void OnDidSaveToWallet(
      const std::string& instrument_id,
      const std::string& address_id,
      const std::vector<wallet::RequiredAction>& required_actions,
      const std::vector<wallet::FormFieldError>& form_field_errors) OVERRIDE;
  virtual void OnWalletError(
      wallet::WalletClient::ErrorType error_type) OVERRIDE;

  
  virtual void OnPersonalDataChanged() OVERRIDE;

  
  virtual void AccountChooserWillShow() OVERRIDE;
  virtual void AccountChoiceChanged() OVERRIDE;
  virtual void AddAccount() OVERRIDE;
  virtual void UpdateAccountChooserView() OVERRIDE;

  
  virtual void OnPassiveSigninSuccess() OVERRIDE;
  virtual void OnPassiveSigninFailure(
      const GoogleServiceAuthError& error) OVERRIDE;
  virtual void OnDidFetchWalletCookieValue(
      const std::string& cookie_value) OVERRIDE;

  
  virtual void AnimationEnded(const gfx::Animation* animation) OVERRIDE;
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;

 protected:
  enum DialogSignedInState {
    NOT_CHECKED,
    REQUIRES_RESPONSE,
    REQUIRES_SIGN_IN,
    REQUIRES_PASSIVE_SIGN_IN,
    SIGNED_IN,
    SIGN_IN_DISABLED,
  };

  
  AutofillDialogControllerImpl(
      content::WebContents* contents,
      const FormData& form_structure,
      const GURL& source_url,
      const base::Callback<void(const FormStructure*)>& callback);

  
  AutofillDialogView* view() { return view_.get(); }
  virtual AutofillDialogView* CreateView();
  const DetailInput* input_showing_popup() const {
    return input_showing_popup_;
  }

  
  virtual PersonalDataManager* GetManager() const;

  
  
  const wallet::WalletClient* GetWalletClient() const;
  virtual wallet::WalletClient* GetWalletClient();

  
  
  void DisableWallet(wallet::WalletClient::ErrorType error_type);

  
  virtual bool IsPayingWithWallet() const;

  
  
  virtual void LoadRiskFingerprintData();
  virtual void OnDidLoadRiskFingerprintData(
      scoped_ptr<risk::Fingerprint> fingerprint);

  
  virtual void OpenTabWithUrl(const GURL& url);

  
  
  virtual bool IsEditingExistingData(DialogSection section) const;

  
  
  bool IsManuallyEditingSection(DialogSection section) const;

  
  virtual void OnWalletSigninError();

  
  bool IsSubmitPausedOn(wallet::RequiredAction action) const;

  
  
  virtual void ShowNewCreditCardBubble(
      scoped_ptr<CreditCard> new_card,
      scoped_ptr<AutofillProfile> billing_profile);

  
  
  void DoFinishSubmit();

  
  
  virtual void SubmitButtonDelayBegin();

  
  
  
  void SubmitButtonDelayEndForTesting();

  
  void ClearLastWalletItemsFetchTimestampForTesting();

  
  AccountChooserModel* AccountChooserModelForTesting();

  
  
  virtual bool IsSignInContinueUrl(const GURL& url, size_t* user_index) const;

  
  DialogSignedInState SignedInState() const;

 private:
  
  void SuggestionsUpdated();

  
  void GetWalletItems();

  
  void HideSignIn();

  
  
  void SignedInStateUpdated();

  
  void OnWalletOrSigninUpdate();

  
  void OnWalletFormFieldError(
      const std::vector<wallet::FormFieldError>& form_field_errors);

  
  void ConstructLegalDocumentsText();

  
  
  void ResetSectionInput(DialogSection section);

  
  void ShowEditUiIfBadSuggestion(DialogSection section);

  
  bool InputWasEdited(ServerFieldType type,
                      const base::string16& value);

  
  FieldValueMap TakeUserInputSnapshot();

  
  
  void RestoreUserInputFromSnapshot(const FieldValueMap& snapshot);

  
  void UpdateSection(DialogSection section);

  
  
  
  
  void UpdateForErrors();

  
  gfx::Image GetGeneratedCardImage(const base::string16& card_number,
                                   const base::string16& name,
                                   const SkColor& gradient_top,
                                   const SkColor& gradient_bottom);

  
  void StartCardScramblingRefresher();

  
  void RefreshCardScramblingOverlay();

  
  void PushOverlayUpdate();

  
  
  
  scoped_ptr<DataModelWrapper> CreateWrapper(DialogSection section);

  
  
  
  const wallet::WalletItems::MaskedInstrument* ActiveInstrument() const;
  const wallet::Address* ActiveShippingAddress() const;

  
  
  void FillOutputForSection(DialogSection section);
  
  
  void FillOutputForSectionWithComparator(DialogSection section,
                                          const InputFieldComparator& compare);

  
  
  bool FormStructureCaresAboutSection(DialogSection section) const;

  
  
  void SetOutputForFieldsOfType(ServerFieldType type,
                                const base::string16& output);

  
  
  base::string16 GetValueFromSection(DialogSection section,
                               ServerFieldType type);

  
  SuggestionsMenuModel* SuggestionsMenuModelForSection(DialogSection section);
  const SuggestionsMenuModel* SuggestionsMenuModelForSection(
      DialogSection section) const;
  
  DialogSection SectionForSuggestionsMenuModel(
      const SuggestionsMenuModel& model);

  
  
  
  
  bool SuggestionTextForSection(DialogSection section,
                                base::string16* vertically_compact,
                                base::string16* horizontally_compact);
  base::string16 RequiredActionTextForSection(DialogSection section) const;
  gfx::Image SuggestionIconForSection(DialogSection section);
  base::string16 ExtraSuggestionTextForSection(DialogSection section) const;
  gfx::Image ExtraSuggestionIconForSection(DialogSection section);

  
  
  
  
  void GetProfileSuggestions(
      ServerFieldType type,
      const base::string16& field_contents,
      const DetailInputs& inputs,
      std::vector<base::string16>* popup_values,
      std::vector<base::string16>* popup_labels,
      std::vector<base::string16>* popup_icons);

  
  DetailInputs* MutableRequestedFieldsForSection(DialogSection section);

  
  void HidePopup();

  
  
  void SetEditingExistingData(DialogSection section, bool editing);

  
  bool IsManuallyEditingAnySection() const;

  
  base::string16 CreditCardNumberValidityMessage(
      const base::string16& number) const;

  
  
  
  
  bool AllSectionsAreValid();

  
  
  
  bool SectionIsValid(DialogSection section);

  
  bool IsCreditCardExpirationValid(const base::string16& year,
                                   const base::string16& month) const;

  
  
  bool IsASuggestionItemKey(const std::string& key) const;

  
  bool ShouldUseBillingForShipping();

  
  bool ShouldSaveDetailsLocally();

  
  
  void SetIsSubmitting(bool submitting);

  
  bool AreLegalDocumentsCurrent() const;

  
  void AcceptLegalTerms();

  
  
  
  void SubmitWithWallet();

  
  scoped_ptr<wallet::Instrument> CreateTransientInstrument();

  
  scoped_ptr<wallet::Address> CreateTransientAddress();

  
  
  void GetFullWallet();

  
  
  void HandleSaveOrUpdateRequiredActions(
      const std::vector<wallet::RequiredAction>& required_actions);

  
  void FinishSubmit();

  
  void PersistAutofillChoice(DialogSection section,
                             const std::string& guid);

  
  
  void GetDefaultAutofillChoice(DialogSection section,
                                std::string* guid);

  
  
  bool GetAutofillChoice(DialogSection section,
                         std::string* guid);

  
  void LogOnFinishSubmitMetrics();

  
  void LogOnCancelMetrics();

  
  void LogEditUiShownMetric(DialogSection section);

  
  void LogSuggestionItemSelectedMetric(const SuggestionsMenuModel& model);

  
  
  void LogDialogLatencyToShow();

  
  
  AutofillMetrics::DialogInitialUserStateMetric GetInitialUserState() const;

  
  
  void MaybeShowCreditCardBubble();

  
  void OnSubmitButtonDelayEnd();

  
  void FetchWalletCookie();

  
  Profile* const profile_;

  
  const AutofillMetrics metric_logger_;
  base::Time dialog_shown_timestamp_;
  AutofillMetrics::DialogInitialUserStateMetric initial_user_state_;

  FormStructure form_structure_;

  
  
  bool invoked_from_same_origin_;

  
  GURL source_url_;

  
  base::Callback<void(const FormStructure*)> callback_;

  
  
  scoped_ptr<AccountChooserModel> account_chooser_model_;

  
  
  scoped_ptr<wallet::WalletSigninHelper> signin_helper_;

  
  wallet::WalletClient wallet_client_;

  
  bool wallet_items_requested_;

  
  
  bool handling_use_wallet_link_click_;

  
  
  bool passive_failed_;

  
  scoped_ptr<wallet::WalletItems> wallet_items_;
  scoped_ptr<wallet::FullWallet> full_wallet_;

  
  
  
  std::string previous_default_instrument_id_;
  std::string previous_default_shipping_address_id_;
  
  
  
  std::string previously_selected_instrument_id_;
  std::string previously_selected_shipping_address_id_;

  
  base::TimeTicks last_wallet_items_fetch_timestamp_;

  
  
  std::string risk_data_;

  
  base::string16 legal_documents_text_;
  
  std::vector<gfx::Range> legal_document_link_ranges_;

  
  
  std::string active_instrument_id_;
  std::string active_address_id_;

  
  DetailInputs requested_cc_fields_;
  DetailInputs requested_billing_fields_;
  DetailInputs requested_cc_billing_fields_;
  DetailInputs requested_shipping_fields_;

  
  MonthComboboxModel cc_exp_month_combobox_model_;
  YearComboboxModel cc_exp_year_combobox_model_;

  
  CountryComboboxModel country_combobox_model_;

  
  SuggestionsMenuModel suggested_cc_;
  SuggestionsMenuModel suggested_billing_;
  SuggestionsMenuModel suggested_cc_billing_;
  SuggestionsMenuModel suggested_shipping_;

  
  std::set<DialogSection> section_editing_state_;

  
  
  bool cares_about_shipping_;

  
  std::vector<PersonalDataManager::GUIDPair> popup_guids_;

  
  
  base::WeakPtr<AutofillPopupControllerImpl> popup_controller_;

  
  
  const DetailInput* input_showing_popup_;

  scoped_ptr<AutofillDialogView> view_;

  
  content::NotificationRegistrar signin_registrar_;

  base::WeakPtrFactory<AutofillDialogControllerImpl> weak_ptr_factory_;

  
  
  
  bool waiting_for_explicit_sign_in_response_;

  
  bool has_accepted_legal_documents_;

  
  
  bool is_submitting_;

  
  
  
  
  bool choose_another_instrument_or_address_;

  
  
  bool wallet_server_validation_recoverable_;

  
  bool data_was_passed_back_;

  typedef std::map<ServerFieldType,
      std::pair<base::string16, base::string16> > TypeErrorInputMap;
  typedef std::map<DialogSection, TypeErrorInputMap> WalletValidationErrors;
  
  WalletValidationErrors wallet_errors_;

  
  scoped_ptr<DialogNotification> wallet_error_notification_;

  
  bool was_ui_latency_logged_;

  
  
  std::string wallet_cookie_value_;

  
  
  
  std::map<DialogSection, std::string> newly_saved_data_model_guids_;

  
  
  
  scoped_ptr<CreditCard> newly_saved_card_;

  
  
  base::string16 backing_card_last_four_;

  
  
  base::OneShotTimer<AutofillDialogControllerImpl> submit_button_delay_timer_;

  
  
  base::string16 scrambled_card_number_;

  
  
  
  base::OneShotTimer<AutofillDialogControllerImpl> card_scrambling_delay_;
  base::RepeatingTimer<AutofillDialogControllerImpl> card_scrambling_refresher_;

  
  
  gfx::LinearAnimation card_generated_animation_;

  
  base::string16 submitted_cardholder_name_;

  DISALLOW_COPY_AND_ASSIGN(AutofillDialogControllerImpl);
};

}  

#endif  
