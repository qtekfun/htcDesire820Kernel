// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_WEBDATA_AUTOFILL_TABLE_H_
#define CHROME_BROWSER_WEBDATA_AUTOFILL_TABLE_H_
#pragma once

#include "base/gtest_prod_util.h"
#include "base/string16.h"
#include "chrome/browser/webdata/web_database_table.h"

#include <vector>

class AutofillChange;
class AutofillEntry;
class AutofillProfile;
class AutofillTableTest;
class CreditCard;

namespace webkit_glue {
struct FormField;
}

class AutofillTable : public WebDatabaseTable {
 public:
  AutofillTable(sql::Connection* db, sql::MetaTable* meta_table)
      : WebDatabaseTable(db, meta_table) {}
  virtual ~AutofillTable() {}
  virtual bool Init();
  virtual bool IsSyncable();

  
  
  
  bool AddFormFieldValues(const std::vector<webkit_glue::FormField>& elements,
                          std::vector<AutofillChange>* changes);

  
  
  
  bool AddFormFieldValue(const webkit_glue::FormField& element,
                         std::vector<AutofillChange>* changes);

  
  
  
  bool GetFormValuesForElementName(const string16& name,
                                   const string16& prefix,
                                   std::vector<string16>* values,
                                   int limit);

  
  
  
  
  
  
  bool RemoveFormElementsAddedBetween(base::Time delete_begin,
                                      base::Time delete_end,
                                      std::vector<AutofillChange>* changes);

  
  
  bool RemoveFormElementForTimeRange(int64 pair_id,
                                     base::Time delete_begin,
                                     base::Time delete_end,
                                     int* how_many);

  
  
  
  bool AddToCountOfFormElement(int64 pair_id, int delta, bool* was_removed);

  
  
  
  bool GetIDAndCountOfFormElement(const webkit_glue::FormField& element,
                                  int64* pair_id,
                                  int* count);

  
  bool GetCountOfFormElement(int64 pair_id, int* count);

  
  bool SetCountOfFormElement(int64 pair_id, int count);

  
  
  bool InsertFormElement(const webkit_glue::FormField& element, int64* pair_id);

  
  bool InsertPairIDAndDate(int64 pair_id, base::Time date_created);

  
  bool RemoveFormElementForID(int64 pair_id);

  
  virtual bool RemoveFormElement(const string16& name, const string16& value);

  
  virtual bool GetAllAutofillEntries(std::vector<AutofillEntry>* entries);

  
  virtual bool GetAutofillTimestamps(const string16& name,
                             const string16& value,
                             std::vector<base::Time>* timestamps);

  
  
  
  virtual bool UpdateAutofillEntries(const std::vector<AutofillEntry>& entries);

  
  virtual bool AddAutofillProfile(const AutofillProfile& profile);

  
  
  virtual bool UpdateAutofillProfile(const AutofillProfile& profile);

  
  virtual bool UpdateAutofillProfileMulti(const AutofillProfile& profile);

  
  
  virtual bool RemoveAutofillProfile(const std::string& guid);

  
  bool GetAutofillProfile(const std::string& guid, AutofillProfile** profile);

  
  virtual bool GetAutofillProfiles(std::vector<AutofillProfile*>* profiles);

  
  bool AddCreditCard(const CreditCard& credit_card);

  
  bool UpdateCreditCard(const CreditCard& credit_card);

  
  
  bool RemoveCreditCard(const std::string& guid);

  
  
  bool GetCreditCard(const std::string& guid, CreditCard** credit_card);

  
  
  virtual bool GetCreditCards(std::vector<CreditCard*>* credit_cards);

  
  
  
  bool RemoveAutofillProfilesAndCreditCardsModifiedBetween(
      base::Time delete_begin,
      base::Time delete_end,
      std::vector<std::string>* profile_guids,
      std::vector<std::string>* credit_card_guids);

  
  
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
                           RemoveAutofillProfilesAndCreditCardsModifiedBetween);
  FRIEND_TEST_ALL_PREFIXES(AutofillTableTest, CreditCard);
  FRIEND_TEST_ALL_PREFIXES(AutofillTableTest, UpdateCreditCard);
  FRIEND_TEST_ALL_PREFIXES(AutofillTableTest,
                           Autofill_GetAllAutofillEntries_OneResult);
  FRIEND_TEST_ALL_PREFIXES(AutofillTableTest,
                           Autofill_GetAllAutofillEntries_TwoDistinct);
  FRIEND_TEST_ALL_PREFIXES(AutofillTableTest,
                           Autofill_GetAllAutofillEntries_TwoSame);

  
  
  bool AddFormFieldValuesTime(
      const std::vector<webkit_glue::FormField>& elements,
      std::vector<AutofillChange>* changes,
      base::Time time);
  bool AddFormFieldValueTime(const webkit_glue::FormField& element,
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

  DISALLOW_COPY_AND_ASSIGN(AutofillTable);
};

#endif  
