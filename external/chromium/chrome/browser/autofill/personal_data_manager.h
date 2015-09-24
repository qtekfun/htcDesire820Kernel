// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOFILL_PERSONAL_DATA_MANAGER_H_
#define CHROME_BROWSER_AUTOFILL_PERSONAL_DATA_MANAGER_H_
#pragma once

#include <set>
#include <vector>

#ifdef ANDROID
#include "base/base_api.h"
#endif
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/observer_list.h"
#include "base/string16.h"
#include "chrome/browser/autofill/autofill_profile.h"
#include "chrome/browser/autofill/credit_card.h"
#include "chrome/browser/autofill/field_types.h"
#include "chrome/browser/sync/profile_sync_service_observer.h"
#include "chrome/browser/webdata/web_data_service.h"

class AutofillManager;
class AutofillMetrics;
class FormStructure;
class Profile;

class
#ifdef ANDROID
BASE_API
#endif
PersonalDataManager
    : public WebDataServiceConsumer,
      public ProfileSyncServiceObserver,
      public base::RefCountedThreadSafe<PersonalDataManager> {
 public:
  
  
  
  class
#ifdef ANDROID
BASE_API
#endif
  Observer {
   public:
    
    
    
    virtual void OnPersonalDataLoaded() = 0;

    
    virtual void OnPersonalDataChanged() {}

   protected:
    virtual ~Observer() {}
  };

  
  virtual void OnWebDataServiceRequestDone(WebDataService::Handle h,
                                           const WDTypedResult* result);

  
  virtual void SetObserver(PersonalDataManager::Observer* observer);

  
  virtual void RemoveObserver(PersonalDataManager::Observer* observer);

  
  virtual void OnStateChanged();

        
  
  
  
  
  bool ImportFormData(const std::vector<const FormStructure*>& form_structures,
                      const CreditCard** credit_card);

  
  virtual void SaveImportedCreditCard(const CreditCard& imported_credit_card);

  
  
  
  
  
  
  
  
  
  
  
  void SetProfiles(std::vector<AutofillProfile>* profiles);

  
  
  void SetCreditCards(std::vector<CreditCard>* credit_cards);

  
  void AddProfile(const AutofillProfile& profile);

  
  void UpdateProfile(const AutofillProfile& profile);

  
  void RemoveProfile(const std::string& guid);

  
  
  AutofillProfile* GetProfileByGUID(const std::string& guid);

  
  void AddCreditCard(const CreditCard& credit_card);

  
  void UpdateCreditCard(const CreditCard& credit_card);

  
  void RemoveCreditCard(const std::string& guid);

  
  
  CreditCard* GetCreditCardByGUID(const std::string& guid);

  
  
  void GetPossibleFieldTypes(const string16& text,
                             FieldTypeSet* possible_types);

  
  bool HasPassword();

  
  virtual bool IsDataLoaded() const;

  
  
  
  
  const std::vector<AutofillProfile*>& profiles();
  virtual const std::vector<AutofillProfile*>& web_profiles();
  virtual const std::vector<CreditCard*>& credit_cards();

  
  
  
  
  
  
  
  
  virtual void Refresh();

  
  void Init(Profile* profile);

  
  static bool IsValidLearnableProfile(const AutofillProfile& profile);

  
  
  
  static bool MergeProfile(
      const AutofillProfile& profile,
      const std::vector<AutofillProfile*>& existing_profiles,
      std::vector<AutofillProfile>* merged_profiles);

 protected:
  
  
  friend class base::RefCountedThreadSafe<PersonalDataManager>;
  friend class AutofillMergeTest;
  friend class PersonalDataManagerTest;
  friend class ProfileImpl;
  friend class ProfileSyncServiceAutofillTest;
#ifdef ANDROID
  friend class ProfileImplAndroid;
#endif

  PersonalDataManager();
  virtual ~PersonalDataManager();

  
  virtual void LoadProfiles();

  
  void LoadAuxiliaryProfiles();

  
  virtual void LoadCreditCards();

  
  
  void ReceiveLoadedProfiles(WebDataService::Handle h,
                             const WDTypedResult* result);

  
  
  void ReceiveLoadedCreditCards(WebDataService::Handle h,
                                const WDTypedResult* result);

  
  
  void CancelPendingQuery(WebDataService::Handle* handle);

  
  virtual void SaveImportedProfile(const AutofillProfile& imported_profile);

  
  void EmptyMigrationTrash();

  
  
  void LogProfileCount() const;

  
  virtual bool IsAutofillEnabled() const;

  
  const AutofillMetrics* metric_logger() const;
  void set_metric_logger(const AutofillMetrics* metric_logger);

  
  Profile* profile_;

  
  bool is_data_loaded_;

  
  ScopedVector<AutofillProfile> web_profiles_;

  
  ScopedVector<AutofillProfile> auxiliary_profiles_;

  
  
  std::vector<AutofillProfile*> profiles_;

  
  ScopedVector<CreditCard> credit_cards_;

  
  
  string16 password_hash_;

  
  
  
  
  WebDataService::Handle pending_profiles_query_;
  WebDataService::Handle pending_creditcards_query_;

  
  ObserverList<Observer> observers_;

 private:
  
  scoped_ptr<const AutofillMetrics> metric_logger_;

  
  mutable bool has_logged_profile_count_;

  DISALLOW_COPY_AND_ASSIGN(PersonalDataManager);
};

#endif  
