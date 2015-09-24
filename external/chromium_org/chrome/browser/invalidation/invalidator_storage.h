// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_INVALIDATION_INVALIDATOR_STORAGE_H_
#define CHROME_BROWSER_INVALIDATION_INVALIDATOR_STORAGE_H_

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/thread_checker.h"
#include "sync/notifier/invalidation_state_tracker.h"
#include "sync/notifier/unacked_invalidation_set.h"

class PrefService;

namespace base {
class DictionaryValue;
class ListValue;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace invalidation {

class InvalidatorStorage : public base::SupportsWeakPtr<InvalidatorStorage>,
                           public syncer::InvalidationStateTracker {
 public:
  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  
  explicit InvalidatorStorage(PrefService* pref_service);
  virtual ~InvalidatorStorage();

  
  virtual void SetInvalidatorClientId(const std::string& client_id) OVERRIDE;
  virtual std::string GetInvalidatorClientId() const OVERRIDE;
  virtual void SetBootstrapData(const std::string& data) OVERRIDE;
  virtual std::string GetBootstrapData() const OVERRIDE;
  virtual void SetSavedInvalidations(
      const syncer::UnackedInvalidationsMap& map) OVERRIDE;
  virtual syncer::UnackedInvalidationsMap GetSavedInvalidations()
      const OVERRIDE;
  virtual void Clear() OVERRIDE;

 private:
  FRIEND_TEST_ALL_PREFIXES(InvalidatorStorageTest, SerializeEmptyMap);
  FRIEND_TEST_ALL_PREFIXES(InvalidatorStorageTest,
                           DeserializeFromListOutOfRange);
  FRIEND_TEST_ALL_PREFIXES(InvalidatorStorageTest,
                           DeserializeFromListInvalidFormat);
  FRIEND_TEST_ALL_PREFIXES(InvalidatorStorageTest,
                           DeserializeFromListWithDuplicates);
  FRIEND_TEST_ALL_PREFIXES(InvalidatorStorageTest,
                           DeserializeFromEmptyList);
  FRIEND_TEST_ALL_PREFIXES(InvalidatorStorageTest, DeserializeFromListBasic);
  FRIEND_TEST_ALL_PREFIXES(InvalidatorStorageTest,
                           DeserializeFromListMissingOptionalValues);
  FRIEND_TEST_ALL_PREFIXES(InvalidatorStorageTest, DeserializeMapOutOfRange);
  FRIEND_TEST_ALL_PREFIXES(InvalidatorStorageTest, DeserializeMapInvalidFormat);
  FRIEND_TEST_ALL_PREFIXES(InvalidatorStorageTest,
                           DeserializeMapEmptyDictionary);
  FRIEND_TEST_ALL_PREFIXES(InvalidatorStorageTest, DeserializeMapBasic);
  FRIEND_TEST_ALL_PREFIXES(InvalidatorStorageTest, MigrateLegacyPreferences);

  base::ThreadChecker thread_checker_;

  PrefService* const pref_service_;

  DISALLOW_COPY_AND_ASSIGN(InvalidatorStorage);
};

}  

#endif  
