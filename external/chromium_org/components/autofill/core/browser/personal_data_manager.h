// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_PERSONAL_DATA_MANAGER_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_PERSONAL_DATA_MANAGER_H_

#include <set>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/observer_list.h"
#include "base/strings/string16.h"
#include "components/autofill/core/browser/autofill_metrics.h"
#include "components/autofill/core/browser/autofill_profile.h"
#include "components/autofill/core/browser/credit_card.h"
#include "components/autofill/core/browser/field_types.h"
#include "components/autofill/core/browser/webdata/autofill_webdata_service.h"
#include "components/autofill/core/browser/webdata/autofill_webdata_service_observer.h"
#include "components/webdata/common/web_data_service_consumer.h"

class PrefService;
class RemoveAutofillTester;

namespace autofill {
class AutofillInteractiveTest;
class AutofillTest;
class FormStructure;
class PersonalDataManagerObserver;
class PersonalDataManagerFactory;
}  

namespace autofill_helper {
void SetProfiles(int, std::vector<autofill::AutofillProfile>*);
void SetCreditCards(int, std::vector<autofill::CreditCard>*);
}  

namespace autofill {

class PersonalDataManager : public WebDataServiceConsumer,
                            public AutofillWebDataServiceObserverOnUIThread {
 public:
  
  
  typedef std::pair<std::string, size_t> GUIDPair;

  explicit PersonalDataManager(const std::string& app_locale);
  virtual ~PersonalDataManager();

  
  
  
  
  void Init(scoped_refptr<AutofillWebDataService> database,
            PrefService* pref_service,
            bool is_off_the_record);

  
  virtual void OnWebDataServiceRequestDone(
      WebDataServiceBase::Handle h,
      const WDTypedResult* result) OVERRIDE;

  
  virtual void AutofillMultipleChanged() OVERRIDE;

  
  virtual void AddObserver(PersonalDataManagerObserver* observer);

  
  virtual void RemoveObserver(PersonalDataManagerObserver* observer);

  
  
  
  
  
  bool ImportFormData(const FormStructure& form,
                      scoped_ptr<CreditCard>* credit_card);

  
  
  virtual std::string SaveImportedProfile(
      const AutofillProfile& imported_profile);

  
  
  virtual std::string SaveImportedCreditCard(
      const CreditCard& imported_credit_card);

  
  void AddProfile(const AutofillProfile& profile);

  
  void UpdateProfile(const AutofillProfile& profile);

  
  virtual void RemoveByGUID(const std::string& guid);

  
  
  
  AutofillProfile* GetProfileByGUID(const std::string& guid);

  
  void AddCreditCard(const CreditCard& credit_card);

  
  void UpdateCreditCard(const CreditCard& credit_card);

  
  
  CreditCard* GetCreditCardByGUID(const std::string& guid);

  
  void GetNonEmptyTypes(ServerFieldTypeSet* non_empty_types);

  
  bool HasPassword();

  
  virtual bool IsDataLoaded() const;

  
  
  
  
  virtual const std::vector<AutofillProfile*>& GetProfiles() const;
  virtual const std::vector<AutofillProfile*>& web_profiles() const;
  virtual const std::vector<CreditCard*>& GetCreditCards() const;

  
  
  
  
  void GetProfileSuggestions(
      const AutofillType& type,
      const base::string16& field_contents,
      bool field_is_autofilled,
      const std::vector<ServerFieldType>& other_field_types,
      std::vector<base::string16>* values,
      std::vector<base::string16>* labels,
      std::vector<base::string16>* icons,
      std::vector<GUIDPair>* guid_pairs);

  
  
  
  void GetCreditCardSuggestions(
      const AutofillType& type,
      const base::string16& field_contents,
      std::vector<base::string16>* values,
      std::vector<base::string16>* labels,
      std::vector<base::string16>* icons,
      std::vector<GUIDPair>* guid_pairs);

  
  
  
  
  
  
  
  
  virtual void Refresh();

  const std::string& app_locale() const { return app_locale_; }

  
  static bool IsValidLearnableProfile(const AutofillProfile& profile,
                                      const std::string& app_locale);

  
  
  
  
  static std::string MergeProfile(
      const AutofillProfile& new_profile,
      const std::vector<AutofillProfile*>& existing_profiles,
      const std::string& app_locale,
      std::vector<AutofillProfile>* merged_profiles);

  
  
  
  virtual const std::string& GetDefaultCountryCodeForNewAddress() const;

 protected:
  
  
  FRIEND_TEST_ALL_PREFIXES(AutofillMetricsTest, FirstMiddleLast);
  FRIEND_TEST_ALL_PREFIXES(AutofillMetricsTest, AutofillIsEnabledAtStartup);
  FRIEND_TEST_ALL_PREFIXES(PersonalDataManagerTest,
                           AggregateExistingAuxiliaryProfile);
  friend class autofill::AutofillInteractiveTest;
  friend class autofill::AutofillTest;
  friend class autofill::PersonalDataManagerFactory;
  friend class PersonalDataManagerTest;
  friend class ProfileSyncServiceAutofillTest;
  friend class ::RemoveAutofillTester;
  friend class TestingAutomationProvider;
  friend struct base::DefaultDeleter<PersonalDataManager>;
  friend void autofill_helper::SetProfiles(
      int, std::vector<autofill::AutofillProfile>*);
  friend void autofill_helper::SetCreditCards(
      int, std::vector<autofill::CreditCard>*);

  
  
  
  
  
  
  
  
  
  
  void SetProfiles(std::vector<AutofillProfile>* profiles);

  
  
  void SetCreditCards(std::vector<CreditCard>* credit_cards);

  
  virtual void LoadProfiles();

  
  virtual void LoadAuxiliaryProfiles() const;

  
  virtual void LoadCreditCards();

  
  
  void ReceiveLoadedProfiles(WebDataServiceBase::Handle h,
                             const WDTypedResult* result);

  
  
  void ReceiveLoadedCreditCards(WebDataServiceBase::Handle h,
                                const WDTypedResult* result);

  
  
  void CancelPendingQuery(WebDataServiceBase::Handle* handle);

  
  
  void LogProfileCount() const;

  
  virtual bool IsAutofillEnabled() const;

  
  const AutofillMetrics* metric_logger() const { return metric_logger_.get(); }

  void set_database(scoped_refptr<AutofillWebDataService> database) {
    database_ = database;
  }

  void set_metric_logger(const AutofillMetrics* metric_logger) {
    metric_logger_.reset(metric_logger);
  }

  void set_pref_service(PrefService* pref_service) {
    pref_service_ = pref_service;
  }

  
  scoped_refptr<AutofillWebDataService> database_;

  
  bool is_data_loaded_;

  
  ScopedVector<AutofillProfile> web_profiles_;

  
  mutable ScopedVector<AutofillProfile> auxiliary_profiles_;

  
  
  mutable std::vector<AutofillProfile*> profiles_;

  
  ScopedVector<CreditCard> credit_cards_;

  
  
  
  
  WebDataServiceBase::Handle pending_profiles_query_;
  WebDataServiceBase::Handle pending_creditcards_query_;

  
  ObserverList<PersonalDataManagerObserver> observers_;

 private:
  
  
  std::string MostCommonCountryCodeFromProfiles() const;

  const std::string app_locale_;

  
  mutable std::string default_country_code_;

  
  scoped_ptr<const AutofillMetrics> metric_logger_;

  
  PrefService* pref_service_;

  
  
  bool is_off_the_record_;

  
  mutable bool has_logged_profile_count_;

  DISALLOW_COPY_AND_ASSIGN(PersonalDataManager);
};

}  

#endif  
