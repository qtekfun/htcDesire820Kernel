// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_WEBDATA_AUTOFILL_TABLE_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_WEBDATA_AUTOFILL_TABLE_H_

#include <vector>

#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_vector.h"
#include "base/strings/string16.h"
#include "components/webdata/common/web_database_table.h"

class WebDatabase;

namespace base {
class Time;
}

namespace autofill {

class AutofillChange;
class AutofillEntry;
class AutofillProfile;
class AutofillTableTest;
class CreditCard;

struct FormFieldData;

class AutofillTable : public WebDatabaseTable {
 public:
  explicit AutofillTable(const std::string& app_locale);
  virtual ~AutofillTable();

  
  static AutofillTable* FromWebDatabase(WebDatabase* db);

  virtual WebDatabaseTable::TypeKey GetTypeKey() const OVERRIDE;
  virtual bool Init(sql::Connection* db, sql::MetaTable* meta_table) OVERRIDE;
  virtual bool IsSyncable() OVERRIDE;
  virtual bool MigrateToVersion(int version,
                                bool* update_compatible_version) OVERRIDE;

  
  
  
  bool AddFormFieldValues(const std::vector<FormFieldData>& elements,
                          std::vector<AutofillChange>* changes);

  
  
  
  bool AddFormFieldValue(const FormFieldData& element,
                         std::vector<AutofillChange>* changes);

  
  
  
  bool GetFormValuesForElementName(const base::string16& name,
                                   const base::string16& prefix,
                                   std::vector<base::string16>* values,
                                   int limit);

  
  bool HasFormElements();

  
  
  
  
  
  
  bool RemoveFormElementsAddedBetween(const base::Time& delete_begin,
                                      const base::Time& delete_end,
                                      std::vector<AutofillChange>* changes);

  
  
  
  
  bool RemoveExpiredFormElements(std::vector<AutofillChange>* changes);

  
  
  bool RemoveFormElementForTimeRange(int64 pair_id,
                                     const base::Time& delete_begin,
                                     const base::Time& delete_end,
                                     int* how_many);

  
  bool AddToCountOfFormElement(int64 pair_id, int delta);

  
  
  
  int CountTimestampsData(int64 pair_id);

  
  
  
  bool GetIDAndCountOfFormElement(const FormFieldData& element,
                                  int64* pair_id,
                                  int* count);

  
  bool GetCountOfFormElement(int64 pair_id, int* count);

  
  bool SetCountOfFormElement(int64 pair_id, int count);

  
  
  bool InsertFormElement(const FormFieldData& element,
                         int64* pair_id);

  
  bool InsertPairIDAndDate(int64 pair_id, const base::Time& date_created);

  
  bool DeleteLastAccess(int64 pair_id);

  
  bool RemoveFormElementForID(int64 pair_id);

  
  virtual bool RemoveFormElement(const base::string16& name,
                                 const base::string16& value);

  
  virtual bool GetAllAutofillEntries(std::vector<AutofillEntry>* entries);

  
  virtual bool GetAutofillTimestamps(const base::string16& name,
                                     const base::string16& value,
                                     std::vector<base::Time>* timestamps);

  
  
  
  virtual bool UpdateAutofillEntries(const std::vector<AutofillEntry>& entries);

  
  virtual bool AddAutofillProfile(const AutofillProfile& profile);

  
  virtual bool UpdateAutofillProfile(const AutofillProfile& profile);

  
  
  virtual bool RemoveAutofillProfile(const std::string& guid);

  
  bool GetAutofillProfile(const std::string& guid, AutofillProfile** profile);

  
  virtual bool GetAutofillProfiles(std::vector<AutofillProfile*>* profiles);

  
  bool AddCreditCard(const CreditCard& credit_card);

  
  bool UpdateCreditCard(const CreditCard& credit_card);

  
  
  bool RemoveCreditCard(const std::string& guid);

  
  
  bool GetCreditCard(const std::string& guid, CreditCard** credit_card);

  
  
  virtual bool GetCreditCards(std::vector<CreditCard*>* credit_cards);

  
  
  
  
  
  
  bool RemoveAutofillDataModifiedBetween(
      const base::Time& delete_begin,
      const base::Time& delete_end,
      std::vector<std::string>* profile_guids,
      std::vector<std::string>* credit_card_guids);

  
  // they were written on or after |delete_begin| and strictly before
  
  
  
  
  bool RemoveOriginURLsModifiedBetween(
      const base::Time& delete_begin,
      const base::Time& delete_end,
      ScopedVector<AutofillProfile>* profiles);

  
  
  bool GetAutofillProfilesInTrash(std::vector<std::string>* guids);

  
  bool EmptyAutofillProfilesTrash();

  
  
  bool ClearAutofillEmptyValueElements();

  
  
  bool AddAutofillGUIDToTrash(const std::string& guid);

  
  bool ClearAutofillProfiles();

  
  bool MigrateToVersion23AddCardNumberEncryptedColumn();
  bool MigrateToVersion24CleanupOversizedStringFields();
  bool MigrateToVersion27UpdateLegacyCreditCards();
  bool MigrateToVersion30AddDateModifed();
  bool MigrateToVersion31AddGUIDToCreditCardsAndProfiles();
  bool MigrateToVersion32UpdateProfilesAndCreditCards();
  bool MigrateToVersion33ProfilesBasedOnFirstName();
  bool MigrateToVersion34ProfilesBasedOnCountryCode();
  bool MigrateToVersion35GreatBritainCountryCodes();
  bool MigrateToVersion37MergeAndCullOlderProfiles();
  bool MigrateToVersion51AddOriginColumn();
  bool MigrateToVersion54AddI18nFieldsAndRemoveDeprecatedFields();

  
  static const size_t kMaxDataLength;

 private:
  FRIEND_TEST_ALL_PREFIXES(AutofillTableTest, Autofill);
  FRIEND_TEST_ALL_PREFIXES(AutofillTableTest, Autofill_AddChanges);
  FRIEND_TEST_ALL_PREFIXES(AutofillTableTest, Autofill_RemoveBetweenChanges);

  FRIEND_TEST_ALL_PREFIXES(AutofillTableTest, Autofill_UpdateDontReplace);
  FRIEND_TEST_ALL_PREFIXES(AutofillTableTest, Autofill_AddFormFieldValues);
  FRIEND_TEST_ALL_PREFIXES(AutofillTableTest, AutofillProfile);
  FRIEND_TEST_ALL_PREFIXES(AutofillTableTest, UpdateAutofillProfile);
  FRIEND_TEST_ALL_PREFIXES(AutofillTableTest, AutofillProfileTrash);
  FRIEND_TEST_ALL_PREFIXES(AutofillTableTest, AutofillProfileTrashInteraction);
  FRIEND_TEST_ALL_PREFIXES(AutofillTableTest,
                           RemoveAutofillDataModifiedBetween);
  FRIEND_TEST_ALL_PREFIXES(AutofillTableTest, CreditCard);
  FRIEND_TEST_ALL_PREFIXES(AutofillTableTest, UpdateCreditCard);
  FRIEND_TEST_ALL_PREFIXES(AutofillTableTest,
                           Autofill_GetAllAutofillEntries_OneResult);
  FRIEND_TEST_ALL_PREFIXES(AutofillTableTest,
                           Autofill_GetAllAutofillEntries_TwoDistinct);
  FRIEND_TEST_ALL_PREFIXES(AutofillTableTest,
                           Autofill_GetAllAutofillEntries_TwoSame);

  
  
  bool AddFormFieldValuesTime(
      const std::vector<FormFieldData>& elements,
      std::vector<AutofillChange>* changes,
      base::Time time);
  bool AddFormFieldValueTime(const FormFieldData& element,
                             std::vector<AutofillChange>* changes,
                             base::Time time);

  
  bool InsertAutofillEntry(const AutofillEntry& entry);

  
  bool IsAutofillProfilesTrashEmpty();

  
  bool IsAutofillGUIDInTrash(const std::string& guid);

  bool InitMainTable();
  bool InitCreditCardsTable();
  bool InitDatesTable();
  bool InitProfilesTable();
  bool InitProfileNamesTable();
  bool InitProfileEmailsTable();
  bool InitProfilePhonesTable();
  bool InitProfileTrashTable();

  
  
  
  
  std::string app_locale_;

  DISALLOW_COPY_AND_ASSIGN(AutofillTable);
};

}  

#endif  
