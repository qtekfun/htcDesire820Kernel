// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_AUTOFILL_MANAGER_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_AUTOFILL_MANAGER_H_

#include <list>
#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "base/time/time.h"
#include "components/autofill/core/browser/autocomplete_history_manager.h"
#include "components/autofill/core/browser/autofill_download.h"
#include "components/autofill/core/browser/autofill_manager_delegate.h"
#include "components/autofill/core/browser/form_structure.h"
#include "components/autofill/core/browser/personal_data_manager.h"
#include "components/autofill/core/common/form_data.h"
#include "components/autofill/core/common/forms_seen_state.h"
#include "third_party/WebKit/public/web/WebFormElement.h"

class GURL;

namespace content {
class RenderViewHost;
class WebContents;
}

namespace gfx {
class Rect;
class RectF;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace autofill {

class AutofillDriver;
class AutofillDataModel;
class AutofillDownloadManager;
class AutofillExternalDelegate;
class AutofillField;
class AutofillManagerDelegate;
class AutofillManagerTestDelegate;
class AutofillMetrics;
class AutofillProfile;
class AutofillType;
class CreditCard;
class FormStructureBrowserTest;

struct FormData;
struct FormFieldData;
struct PasswordFormFillData;

class AutofillManager : public AutofillDownloadManager::Observer {
 public:
  enum AutofillDownloadManagerState {
    ENABLE_AUTOFILL_DOWNLOAD_MANAGER,
    DISABLE_AUTOFILL_DOWNLOAD_MANAGER,
  };

  
  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  AutofillManager(AutofillDriver* driver,
                  autofill::AutofillManagerDelegate* delegate,
                  const std::string& app_locale,
                  AutofillDownloadManagerState enable_download_manager);
  virtual ~AutofillManager();

  
  void SetExternalDelegate(AutofillExternalDelegate* delegate);

  
  virtual void OnFillAutofillFormData(int query_id,
                                      const FormData& form,
                                      const FormFieldData& field,
                                      int unique_id);
  void OnDidShowAutofillSuggestions(bool is_new_popup);
  void OnDidFillAutofillFormData(const base::TimeTicks& timestamp);
  void OnShowAutofillDialog();
  void OnDidPreviewAutofillFormData();

  
  
  void RemoveAutofillProfileOrCreditCard(int unique_id);

  
  void RemoveAutocompleteEntry(const base::string16& name,
                               const base::string16& value);

  
  const std::vector<FormStructure*>& GetFormStructures();

  
  void RequestAutocompleteDialogClosed();

  autofill::AutofillManagerDelegate* delegate() const {
    return manager_delegate_;
  }

  const std::string& app_locale() const { return app_locale_; }

  
  void SetTestDelegate(autofill::AutofillManagerTestDelegate* delegate);

  void OnFormsSeen(const std::vector<FormData>& forms,
                   const base::TimeTicks& timestamp,
                   autofill::FormsSeenState state);

  
  
  
  bool OnFormSubmitted(const FormData& form,
                       const base::TimeTicks& timestamp);

  void OnTextFieldDidChange(const FormData& form,
                            const FormFieldData& field,
                            const base::TimeTicks& timestamp);

  
  void OnQueryFormFieldAutofill(int query_id,
                                const FormData& form,
                                const FormFieldData& field,
                                const gfx::RectF& bounding_box,
                                bool display_warning);
  void OnDidEndTextFieldEditing();
  void OnHideAutofillUI();
  void OnAddPasswordFormMapping(
      const FormFieldData& form,
      const PasswordFormFillData& fill_data);
  void OnShowPasswordSuggestions(
      const FormFieldData& field,
      const gfx::RectF& bounds,
      const std::vector<base::string16>& suggestions,
      const std::vector<base::string16>& realms);
  void OnSetDataList(const std::vector<base::string16>& values,
                     const std::vector<base::string16>& labels);

  
  
  
  
  
  
  
  
  bool UploadPasswordGenerationForm(const FormData& form);

  
  virtual void Reset();

  
  virtual bool IsAutofillEnabled() const;

 protected:
  
  AutofillManager(AutofillDriver* driver,
                  autofill::AutofillManagerDelegate* delegate,
                  PersonalDataManager* personal_data);

  
  virtual void UploadFormData(const FormStructure& submitted_form);

  
  
  virtual void UploadFormDataAsyncCallback(
      const FormStructure* submitted_form,
      const base::TimeTicks& load_time,
      const base::TimeTicks& interaction_time,
      const base::TimeTicks& submission_time);

  
  
  virtual int GUIDToID(const PersonalDataManager::GUIDPair& guid) const;
  virtual const PersonalDataManager::GUIDPair IDToGUID(int id) const;

  
  
  int PackGUIDs(const PersonalDataManager::GUIDPair& cc_guid,
                const PersonalDataManager::GUIDPair& profile_guid) const;
  void UnpackGUIDs(int id,
                   PersonalDataManager::GUIDPair* cc_guid,
                   PersonalDataManager::GUIDPair* profile_guid) const;

  const AutofillMetrics* metric_logger() const { return metric_logger_.get(); }
  void set_metric_logger(const AutofillMetrics* metric_logger);

  ScopedVector<FormStructure>* form_structures() { return &form_structures_; }

  
  AutofillExternalDelegate* external_delegate() {
    return external_delegate_;
  }

 private:
  
  virtual void OnLoadedServerPredictions(
      const std::string& response_xml) OVERRIDE;

  
  bool RefreshDataModels() const;

  
  
  
  bool GetProfileOrCreditCard(int unique_id,
                              const AutofillDataModel** data_model,
                              size_t* variant) const WARN_UNUSED_RESULT;

  
  
  bool FindCachedForm(const FormData& form,
                      FormStructure** form_structure) const WARN_UNUSED_RESULT;

  
  
  
  
  bool GetCachedFormAndField(const FormData& form,
                             const FormFieldData& field,
                             FormStructure** form_structure,
                             AutofillField** autofill_field) WARN_UNUSED_RESULT;

  
  
  // NULL if no cached version exists.  The updated form is then written into
  
  bool UpdateCachedForm(const FormData& live_form,
                        const FormStructure* cached_form,
                        FormStructure** updated_form) WARN_UNUSED_RESULT;

  
  
  
  void GetProfileSuggestions(FormStructure* form,
                             const FormFieldData& field,
                             const AutofillType& type,
                             std::vector<base::string16>* values,
                             std::vector<base::string16>* labels,
                             std::vector<base::string16>* icons,
                             std::vector<int>* unique_ids) const;

  
  
  void GetCreditCardSuggestions(const FormFieldData& field,
                                const AutofillType& type,
                                std::vector<base::string16>* values,
                                std::vector<base::string16>* labels,
                                std::vector<base::string16>* icons,
                                std::vector<int>* unique_ids) const;

  
  void ParseForms(const std::vector<FormData>& forms);

  
  void ImportFormData(const FormStructure& submitted_form);

  
  
  
  void UpdateInitialInteractionTimestamp(
      const base::TimeTicks& interaction_timestamp);

  
  bool ShouldUploadForm(const FormStructure& form);

  
  
  AutofillDriver* driver_;

  autofill::AutofillManagerDelegate* const manager_delegate_;

  std::string app_locale_;

  
  
  
  
  PersonalDataManager* personal_data_;

  std::list<std::string> autofilled_form_signatures_;

  
  
  scoped_ptr<AutofillDownloadManager> download_manager_;

  
  scoped_ptr<AutocompleteHistoryManager> autocomplete_history_manager_;

  
  scoped_ptr<const AutofillMetrics> metric_logger_;
  
  bool has_logged_autofill_enabled_;
  
  bool has_logged_address_suggestions_count_;
  
  bool did_show_suggestions_;
  
  
  bool user_did_type_;
  
  bool user_did_autofill_;
  
  bool user_did_edit_autofilled_field_;
  
  base::TimeTicks forms_loaded_timestamp_;
  
  
  base::TimeTicks initial_interaction_timestamp_;

  
  ScopedVector<FormStructure> form_structures_;

  
  mutable std::map<PersonalDataManager::GUIDPair, int> guid_id_map_;
  mutable std::map<int, PersonalDataManager::GUIDPair> id_guid_map_;

  
  
  AutofillExternalDelegate* external_delegate_;

  
  autofill::AutofillManagerTestDelegate* test_delegate_;

  base::WeakPtrFactory<AutofillManager> weak_ptr_factory_;

  friend class AutofillManagerTest;
  friend class autofill::FormStructureBrowserTest;
  FRIEND_TEST_ALL_PREFIXES(AutofillManagerTest,
                           DeterminePossibleFieldTypesForUpload);
  FRIEND_TEST_ALL_PREFIXES(AutofillManagerTest,
                           DeterminePossibleFieldTypesForUploadStressTest);
  FRIEND_TEST_ALL_PREFIXES(AutofillManagerTest,
                           DisabledAutofillDispatchesError);
  FRIEND_TEST_ALL_PREFIXES(AutofillMetricsTest, AddressSuggestionsCount);
  FRIEND_TEST_ALL_PREFIXES(AutofillMetricsTest, AutofillIsEnabledAtPageLoad);
  FRIEND_TEST_ALL_PREFIXES(AutofillMetricsTest, DeveloperEngagement);
  FRIEND_TEST_ALL_PREFIXES(AutofillMetricsTest, FormFillDuration);
  FRIEND_TEST_ALL_PREFIXES(AutofillMetricsTest,
                           NoQualityMetricsForNonAutofillableForms);
  FRIEND_TEST_ALL_PREFIXES(AutofillMetricsTest, QualityMetrics);
  FRIEND_TEST_ALL_PREFIXES(AutofillMetricsTest, QualityMetricsForFailure);
  FRIEND_TEST_ALL_PREFIXES(AutofillMetricsTest, QualityMetricsWithExperimentId);
  FRIEND_TEST_ALL_PREFIXES(AutofillMetricsTest, SaneMetricsWithCacheMismatch);
  FRIEND_TEST_ALL_PREFIXES(AutofillManagerTest, TestExternalDelegate);
  FRIEND_TEST_ALL_PREFIXES(AutofillManagerTest,
                           TestTabContentsWithExternalDelegate);
  FRIEND_TEST_ALL_PREFIXES(AutofillMetricsTest,
                           UserHappinessFormLoadAndSubmission);
  FRIEND_TEST_ALL_PREFIXES(AutofillMetricsTest, UserHappinessFormInteraction);
  FRIEND_TEST_ALL_PREFIXES(AutofillManagerTest,
                           FormSubmittedAutocompleteEnabled);
  DISALLOW_COPY_AND_ASSIGN(AutofillManager);
};

}  

#endif  
