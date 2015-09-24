// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_AUTOFILL_METRICS_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_AUTOFILL_METRICS_H_

#include <stddef.h>
#include <string>

#include "base/basictypes.h"
#include "components/autofill/core/browser/autofill_manager_delegate.h"
#include "components/autofill/core/browser/field_types.h"

namespace base {
class TimeDelta;
}

namespace autofill {

class AutofillMetrics {
 public:
  enum DeveloperEngagementMetric {
    
    FILLABLE_FORM_PARSED = 0,
    
    
    
    FILLABLE_FORM_CONTAINS_TYPE_HINTS,
    NUM_DEVELOPER_ENGAGEMENT_METRICS,
  };

  
  enum DialogDismissalAction {
    DIALOG_ACCEPTED = 0,  
    DIALOG_CANCELED,      
  };

  
  enum DialogDismissalState {
    
    DIALOG_ACCEPTED_EXISTING_DATA,
    
    DIALOG_ACCEPTED_SAVE_TO_WALLET,
    
    DIALOG_ACCEPTED_SAVE_TO_AUTOFILL,
    
    DIALOG_ACCEPTED_NO_SAVE,
    
    DIALOG_CANCELED_NO_EDITS,
    
    DIALOG_CANCELED_NO_INVALID_FIELDS,
    
    DIALOG_CANCELED_WITH_INVALID_FIELDS,
    
    DIALOG_CANCELED_DURING_SIGNIN,
    NUM_DIALOG_DISMISSAL_STATES
  };

  
  
  enum DialogInitialUserStateMetric {
    
    
    DIALOG_USER_STATE_UNKNOWN = 0,
    
    DIALOG_USER_NOT_SIGNED_IN_NO_AUTOFILL,
    
    DIALOG_USER_NOT_SIGNED_IN_HAS_AUTOFILL,
    
    DIALOG_USER_SIGNED_IN_NO_WALLET_NO_AUTOFILL,
    
    DIALOG_USER_SIGNED_IN_NO_WALLET_HAS_AUTOFILL,
    
    DIALOG_USER_SIGNED_IN_HAS_WALLET_NO_AUTOFILL,
    
    DIALOG_USER_SIGNED_IN_HAS_WALLET_HAS_AUTOFILL,
    NUM_DIALOG_INITIAL_USER_STATE_METRICS
  };

  
  
  enum DialogPopupEvent {
    
    DIALOG_POPUP_SHOWN = 0,
    
    DIALOG_POPUP_FORM_FILLED,
    NUM_DIALOG_POPUP_EVENTS
  };

  
  
  enum DialogSecurityMetric {
    
    SECURITY_METRIC_DIALOG_SHOWN = 0,
    
    SECURITY_METRIC_CREDIT_CARD_OVER_HTTP,
    
    
    SECURITY_METRIC_CROSS_ORIGIN_FRAME,
    NUM_DIALOG_SECURITY_METRICS
  };

  
  enum DialogUiEvent {
    
    DIALOG_UI_SHOWN = 0,

    
    DIALOG_UI_ACCEPTED,
    DIALOG_UI_CANCELED,

    
    
    DIALOG_UI_ACCOUNT_CHOOSER_SWITCHED_TO_AUTOFILL,
    
    DIALOG_UI_ACCOUNT_CHOOSER_SWITCHED_TO_WALLET,
    
    DIALOG_UI_ACCOUNT_CHOOSER_SWITCHED_WALLET_ACCOUNT,

    
    DIALOG_UI_SIGNIN_SHOWN,

    
    DEPRECATED_DIALOG_UI_EMAIL_SELECTED_SUGGESTION_CHANGED,
    DIALOG_UI_BILLING_SELECTED_SUGGESTION_CHANGED,
    DIALOG_UI_CC_BILLING_SELECTED_SUGGESTION_CHANGED,
    DIALOG_UI_SHIPPING_SELECTED_SUGGESTION_CHANGED,
    DIALOG_UI_CC_SELECTED_SUGGESTION_CHANGED,

    
    DEPRECATED_DIALOG_UI_EMAIL_EDIT_UI_SHOWN,
    DEPRECATED_DIALOG_UI_BILLING_EDIT_UI_SHOWN,
    DEPRECATED_DIALOG_UI_CC_BILLING_EDIT_UI_SHOWN,
    DEPRECATED_DIALOG_UI_SHIPPING_EDIT_UI_SHOWN,
    DEPRECATED_DIALOG_UI_CC_EDIT_UI_SHOWN,

    
    DEPRECATED_DIALOG_UI_EMAIL_ITEM_ADDED,
    DIALOG_UI_BILLING_ITEM_ADDED,
    DIALOG_UI_CC_BILLING_ITEM_ADDED,
    DIALOG_UI_SHIPPING_ITEM_ADDED,
    DIALOG_UI_CC_ITEM_ADDED,

    
    
    DIALOG_UI_ACCOUNT_CHOOSER_TRIED_TO_ADD_ACCOUNT,

    NUM_DIALOG_UI_EVENTS
  };

  enum InfoBarMetric {
    INFOBAR_SHOWN = 0,  
                        
    INFOBAR_ACCEPTED,   
    INFOBAR_DENIED,     
    INFOBAR_IGNORED,    
                        
    NUM_INFO_BAR_METRICS,
  };

  
  
  
  
  enum FieldTypeQualityMetric {
    TYPE_UNKNOWN = 0,  
    TYPE_MATCH,        
    TYPE_MISMATCH,     
    NUM_FIELD_TYPE_QUALITY_METRICS,
  };

  enum QualityMetric {
    
    FIELD_SUBMITTED = 0,

    
    
    FIELD_AUTOFILLED,

    
    
    
    FIELD_NOT_AUTOFILLED,

    
    NOT_AUTOFILLED_HEURISTIC_TYPE_UNKNOWN,
    NOT_AUTOFILLED_HEURISTIC_TYPE_MATCH,
    NOT_AUTOFILLED_HEURISTIC_TYPE_MISMATCH,
    NOT_AUTOFILLED_SERVER_TYPE_UNKNOWN,
    NOT_AUTOFILLED_SERVER_TYPE_MATCH,
    NOT_AUTOFILLED_SERVER_TYPE_MISMATCH,
    NUM_QUALITY_METRICS,
  };

  
  
  enum ServerQueryMetric {
    QUERY_SENT = 0,           
    QUERY_RESPONSE_RECEIVED,  
    QUERY_RESPONSE_PARSED,    

    
    
    QUERY_RESPONSE_MATCHED_LOCAL_HEURISTICS,

    
    
    QUERY_RESPONSE_OVERRODE_LOCAL_HEURISTICS,

    
    
    QUERY_RESPONSE_WITH_NO_LOCAL_HEURISTICS,
    NUM_SERVER_QUERY_METRICS,
  };

  
  
  
  
  
  enum UserHappinessMetric {
    
    FORMS_LOADED,
    
    
    
    SUBMITTED_FILLABLE_FORM_AUTOFILLED_ALL,
    
    
    SUBMITTED_FILLABLE_FORM_AUTOFILLED_SOME,
    
    SUBMITTED_FILLABLE_FORM_AUTOFILLED_NONE,
    
    SUBMITTED_NON_FILLABLE_FORM,

    
    USER_DID_TYPE,
    
    SUGGESTIONS_SHOWN,
    
    SUGGESTIONS_SHOWN_ONCE,
    
    USER_DID_AUTOFILL,
    
    USER_DID_AUTOFILL_ONCE,
    
    USER_DID_EDIT_AUTOFILLED_FIELD,
    
    USER_DID_EDIT_AUTOFILLED_FIELD_ONCE,
    NUM_USER_HAPPINESS_METRICS,
  };

  
  
  enum WalletApiCallMetric {
    UNKNOWN_API_CALL,  
    ACCEPT_LEGAL_DOCUMENTS,
    AUTHENTICATE_INSTRUMENT,
    GET_FULL_WALLET,
    GET_WALLET_ITEMS,
    SAVE_TO_WALLET,
    NUM_WALLET_API_CALLS
  };

  
  
  enum WalletErrorMetric {
    
    WALLET_ERROR_BASELINE_ISSUED_REQUEST = 0,
    
    
    WALLET_FATAL_ERROR_DEPRECATED,
    
    WALLET_MALFORMED_RESPONSE,
    
    WALLET_NETWORK_ERROR,
    
    WALLET_BAD_REQUEST,
    
    WALLET_BUYER_ACCOUNT_ERROR,
    
    WALLET_INTERNAL_ERROR,
    
    WALLET_INVALID_PARAMS,
    
    WALLET_SERVICE_UNAVAILABLE,
    
    
    WALLET_SPENDING_LIMIT_EXCEEDED_DEPRECATED,
    
    WALLET_UNSUPPORTED_API_VERSION,
    
    WALLET_UNKNOWN_ERROR,
    
    
    WALLET_UNSUPPORTED_MERCHANT,
    
    WALLET_BUYER_LEGAL_ADDRESS_NOT_SUPPORTED,
    
    WALLET_UNVERIFIED_KNOW_YOUR_CUSTOMER_STATUS,
    NUM_WALLET_ERROR_METRICS
  };

  
  
  
  
  
  enum WalletRequiredActionMetric {
    
    WALLET_REQUIRED_ACTION_BASELINE_ISSUED_REQUEST = 0,
    
    UNKNOWN_REQUIRED_ACTION,  
    GAIA_AUTH,
    PASSIVE_GAIA_AUTH,
    SETUP_WALLET,
    ACCEPT_TOS,
    UPDATE_EXPIRATION_DATE,
    UPGRADE_MIN_ADDRESS,
    CHOOSE_ANOTHER_INSTRUMENT_OR_ADDRESS,
    VERIFY_CVV,
    INVALID_FORM_FIELD,
    REQUIRE_PHONE_NUMBER,
    NUM_WALLET_REQUIRED_ACTIONS
  };

  AutofillMetrics();
  virtual ~AutofillMetrics();

  virtual void LogCreditCardInfoBarMetric(InfoBarMetric metric) const;

  virtual void LogDeveloperEngagementMetric(
      DeveloperEngagementMetric metric) const;

  virtual void LogHeuristicTypePrediction(
      FieldTypeQualityMetric metric,
      ServerFieldType field_type,
      const std::string& experiment_id) const;
  virtual void LogOverallTypePrediction(
      FieldTypeQualityMetric metric,
      ServerFieldType field_type,
      const std::string& experiment_id) const;
  virtual void LogServerTypePrediction(FieldTypeQualityMetric metric,
                                       ServerFieldType field_type,
                                       const std::string& experiment_id) const;

  virtual void LogQualityMetric(QualityMetric metric,
                                const std::string& experiment_id) const;

  virtual void LogServerQueryMetric(ServerQueryMetric metric) const;

  virtual void LogUserHappinessMetric(UserHappinessMetric metric) const;

  
  virtual void LogDialogDismissalState(DialogDismissalState state) const;

  
  
  
  virtual void LogDialogInitialUserState(
      DialogInitialUserStateMetric user_type) const;

  
  
  virtual void LogDialogLatencyToShow(const base::TimeDelta& duration) const;

  
  virtual void LogDialogPopupEvent(DialogPopupEvent event) const;

  
  virtual void LogDialogSecurityMetric(DialogSecurityMetric metric) const;

  
  
  
  
  virtual void LogDialogUiDuration(
      const base::TimeDelta& duration,
      DialogDismissalAction dismissal_action) const;

  
  virtual void LogDialogUiEvent(DialogUiEvent event) const;

  
  virtual void LogWalletErrorMetric(WalletErrorMetric metric) const;

  
  virtual void LogWalletApiCallDuration(
      WalletApiCallMetric metric,
      const base::TimeDelta& duration) const;

  
  virtual void LogWalletMalformedResponseMetric(
      WalletApiCallMetric metric) const;

  
  virtual void LogWalletRequiredActionMetric(
      WalletRequiredActionMetric required_action) const;

  
  virtual void LogWalletResponseCode(int response_code) const;

  
  
  virtual void LogFormFillDurationFromLoadWithAutofill(
      const base::TimeDelta& duration) const;

  
  
  
  virtual void LogFormFillDurationFromLoadWithoutAutofill(
      const base::TimeDelta& duration) const;

  
  
  
  virtual void LogFormFillDurationFromInteractionWithAutofill(
      const base::TimeDelta& duration) const;

  
  
  
  virtual void LogFormFillDurationFromInteractionWithoutAutofill(
      const base::TimeDelta& duration) const;

  
  virtual void LogIsAutofillEnabledAtPageLoad(bool enabled) const;

  
  virtual void LogIsAutofillEnabledAtStartup(bool enabled) const;

  
  virtual void LogStoredProfileCount(size_t num_profiles) const;

  
  
  virtual void LogAddressSuggestionsCount(size_t num_suggestions) const;

  
  virtual void LogServerExperimentIdForQuery(
      const std::string& experiment_id) const;

  
  virtual void LogServerExperimentIdForUpload(
      const std::string& experiment_id) const;

 private:
  DISALLOW_COPY_AND_ASSIGN(AutofillMetrics);
};

}  

#endif  
