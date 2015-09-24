// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOFILL_AUTOFILL_MANAGER_H_
#define CHROME_BROWSER_AUTOFILL_AUTOFILL_MANAGER_H_
#pragma once

#include <list>
#include <map>
#include <string>
#include <vector>

#ifdef ANDROID
#include "base/base_api.h"
#endif
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/string16.h"
#include "chrome/browser/autofill/autofill_download.h"
#include "chrome/browser/autofill/field_types.h"
#include "chrome/browser/autofill/form_structure.h"
#ifndef ANDROID
#include "content/browser/tab_contents/navigation_controller.h"
#endif
#include "content/browser/tab_contents/tab_contents_observer.h"

class AutofillField;
class AutofillProfile;
class AutofillMetrics;
class CreditCard;
class PersonalDataManager;
class PrefService;
class RenderViewHost;
class TabContents;

#ifdef ANDROID
class AutoFillHost;
#endif

struct ViewHostMsg_FrameNavigate_Params;

namespace IPC {
class Message;
}

namespace webkit_glue {
struct FormData;
struct FormField;
}

class
#ifdef ANDROID
BASE_API
#endif
AutofillManager : public TabContentsObserver,
                        public AutofillDownloadManager::Observer {
 public:
  explicit AutofillManager(TabContents* tab_contents);
  virtual ~AutofillManager();

#ifndef ANDROID
  
  static void RegisterBrowserPrefs(PrefService* prefs);
#endif

#ifndef ANDROID
  
  static void RegisterUserPrefs(PrefService* prefs);
#endif

#ifndef ANDROID
  
  virtual void DidNavigateMainFramePostCommit(
      const NavigationController::LoadCommittedDetails& details,
      const ViewHostMsg_FrameNavigate_Params& params);
  virtual bool OnMessageReceived(const IPC::Message& message);
#endif

  
  virtual void OnLoadedAutofillHeuristics(const std::string& heuristic_xml);
  virtual void OnUploadedAutofillHeuristics(const std::string& form_signature);
  virtual void OnHeuristicsRequestError(
      const std::string& form_signature,
      AutofillDownloadManager::AutofillRequestType request_type,
      int http_error);

  
  virtual bool IsAutofillEnabled() const;

  
  void ImportFormData(const FormStructure& submitted_form);

  
  void UploadFormData(const FormStructure& submitted_form);

  
  void Reset();

#ifdef ANDROID
  void OnFormsSeenWrapper(const std::vector<webkit_glue::FormData>& forms) {
    OnFormsSeen(forms);
  }

  bool OnQueryFormFieldAutoFillWrapper(const webkit_glue::FormData& form,
                                       const webkit_glue::FormField& field) {
    return OnQueryFormFieldAutofill(0, form, field);
  }

  void OnFillAutoFillFormDataWrapper(int query_id,
                                     const webkit_glue::FormData& form,
                                     const webkit_glue::FormField& field,
                                     int unique_id) {
    OnFillAutofillFormData(query_id, form, field, unique_id);
  }
#endif

 protected:
  

  
  
  
  typedef std::pair<std::string, size_t> GUIDPair;

  AutofillManager(TabContents* tab_contents,
                  PersonalDataManager* personal_data);

  void set_personal_data_manager(PersonalDataManager* personal_data) {
    personal_data_ = personal_data;
  }

  const AutofillMetrics* metric_logger() const { return metric_logger_.get(); }
  void set_metric_logger(const AutofillMetrics* metric_logger);

  ScopedVector<FormStructure>* form_structures() { return &form_structures_; }

  
  
  virtual int GUIDToID(const GUIDPair& guid);
  virtual const GUIDPair IDToGUID(int id);

  
  
  int PackGUIDs(const GUIDPair& cc_guid, const GUIDPair& profile_guid);
  void UnpackGUIDs(int id, GUIDPair* cc_guid, GUIDPair* profile_guid);

 private:
  void OnFormSubmitted(const webkit_glue::FormData& form);
  void OnFormsSeen(const std::vector<webkit_glue::FormData>& forms);
#ifdef ANDROID
  bool
#else
  void
#endif
  OnQueryFormFieldAutofill(int query_id,
                                const webkit_glue::FormData& form,
                                const webkit_glue::FormField& field);
  void OnFillAutofillFormData(int query_id,
                              const webkit_glue::FormData& form,
                              const webkit_glue::FormField& field,
                              int unique_id);
  void OnShowAutofillDialog();
  void OnDidFillAutofillFormData();
  void OnDidShowAutofillSuggestions();

  
  
  bool GetHost(const std::vector<AutofillProfile*>& profiles,
               const std::vector<CreditCard*>& credit_cards,
#ifdef ANDROID
               AutoFillHost** host) WARN_UNUSED_RESULT;
#else
               RenderViewHost** host) const WARN_UNUSED_RESULT;
#endif

  
  
  bool FindCachedForm(const webkit_glue::FormData& form,
                      FormStructure** form_structure) const WARN_UNUSED_RESULT;

  
  
  
  bool FindCachedFormAndField(
      const webkit_glue::FormData& form,
      const webkit_glue::FormField& field,
      FormStructure** form_structure,
      AutofillField** autofill_field) WARN_UNUSED_RESULT;

  
  
  
  void GetProfileSuggestions(FormStructure* form,
                             const webkit_glue::FormField& field,
                             AutofillFieldType type,
                             std::vector<string16>* values,
                             std::vector<string16>* labels,
                             std::vector<string16>* icons,
                             std::vector<int>* unique_ids);

  
  
  void GetCreditCardSuggestions(FormStructure* form,
                                const webkit_glue::FormField& field,
                                AutofillFieldType type,
                                std::vector<string16>* values,
                                std::vector<string16>* labels,
                                std::vector<string16>* icons,
                                std::vector<int>* unique_ids);

  
  
  void FillCreditCardFormField(const CreditCard* credit_card,
                               AutofillFieldType type,
                               webkit_glue::FormField* field);

  
  
  void FillFormField(const AutofillProfile* profile,
                     AutofillFieldType type,
                     size_t variant,
                     webkit_glue::FormField* field);

  
  
  
  void FillPhoneNumberField(const AutofillProfile* profile,
                            AutofillFieldType type,
                            size_t variant,
                            webkit_glue::FormField* field);

  
  void ParseForms(const std::vector<webkit_glue::FormData>& forms);

  
  
  void DeterminePossibleFieldTypesForUpload(FormStructure* submitted_form);

  
  
  
  
  PersonalDataManager* personal_data_;

  std::list<std::string> autofilled_forms_signatures_;
  
  AutofillDownloadManager download_manager_;

  
  
  
  
  bool disable_download_manager_requests_;

  
  scoped_ptr<const AutofillMetrics> metric_logger_;

  
  bool has_logged_autofill_enabled_;

  
  bool has_logged_address_suggestions_count_;

  
  ScopedVector<FormStructure> form_structures_;

  
  std::map<GUIDPair, int> guid_id_map_;
  std::map<int, GUIDPair> id_guid_map_;

  friend class AutofillManagerTest;
  friend class FormStructureBrowserTest;
  FRIEND_TEST_ALL_PREFIXES(AutofillManagerTest, FillCreditCardForm);
  FRIEND_TEST_ALL_PREFIXES(AutofillManagerTest,
                           FillCreditCardFormNoYearNoMonth);
  FRIEND_TEST_ALL_PREFIXES(AutofillManagerTest, FillCreditCardFormYearNoMonth);
  FRIEND_TEST_ALL_PREFIXES(AutofillManagerTest, FillCreditCardFormNoYearMonth);
  FRIEND_TEST_ALL_PREFIXES(AutofillManagerTest, FillCreditCardFormYearMonth);
  FRIEND_TEST_ALL_PREFIXES(AutofillManagerTest, FillAddressForm);
  FRIEND_TEST_ALL_PREFIXES(AutofillManagerTest, FillAddressAndCreditCardForm);
  FRIEND_TEST_ALL_PREFIXES(AutofillManagerTest, FillFormWithMultipleSections);
  FRIEND_TEST_ALL_PREFIXES(AutofillManagerTest, FillFormWithMultipleEmails);
  FRIEND_TEST_ALL_PREFIXES(AutofillManagerTest, FillAutofilledForm);
  FRIEND_TEST_ALL_PREFIXES(AutofillManagerTest, FillPhoneNumber);
  FRIEND_TEST_ALL_PREFIXES(AutofillManagerTest, FormChangesRemoveField);
  FRIEND_TEST_ALL_PREFIXES(AutofillManagerTest, FormChangesAddField);
  FRIEND_TEST_ALL_PREFIXES(AutofillManagerTest, FormSubmitted);
  FRIEND_TEST_ALL_PREFIXES(AutofillManagerTest, FormSubmittedServerTypes);
  FRIEND_TEST_ALL_PREFIXES(AutofillMetricsTest, AddressSuggestionsCount);
  FRIEND_TEST_ALL_PREFIXES(AutofillMetricsTest, AutofillIsEnabledAtPageLoad);
  FRIEND_TEST_ALL_PREFIXES(AutofillMetricsTest,
                           NoQualityMetricsForNonAutofillableForms);
  FRIEND_TEST_ALL_PREFIXES(AutofillMetricsTest, QualityMetrics);
  FRIEND_TEST_ALL_PREFIXES(AutofillMetricsTest, QualityMetricsForFailure);
  FRIEND_TEST_ALL_PREFIXES(AutofillMetricsTest, QualityMetricsWithExperimentId);
  FRIEND_TEST_ALL_PREFIXES(AutofillMetricsTest, SaneMetricsWithCacheMismatch);

  DISALLOW_COPY_AND_ASSIGN(AutofillManager);
};

#endif  
