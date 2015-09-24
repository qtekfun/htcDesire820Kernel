// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_PREFS_TESTING_PREF_SERVICE_H_
#define BASE_PREFS_TESTING_PREF_SERVICE_H_

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/prefs/pref_registry.h"
#include "base/prefs/pref_service.h"
#include "base/prefs/testing_pref_store.h"

class PrefNotifierImpl;
class PrefRegistrySimple;
class TestingPrefStore;

template <class SuperPrefService, class ConstructionPrefRegistry>
class TestingPrefServiceBase : public SuperPrefService {
 public:
  virtual ~TestingPrefServiceBase();

  
  
  const base::Value* GetManagedPref(const char* path) const;

  
  
  void SetManagedPref(const char* path, base::Value* value);

  
  
  void RemoveManagedPref(const char* path);

  
  const base::Value* GetUserPref(const char* path) const;
  void SetUserPref(const char* path, base::Value* value);
  void RemoveUserPref(const char* path);

  
  const base::Value* GetRecommendedPref(const char* path) const;
  void SetRecommendedPref(const char* path, base::Value* value);
  void RemoveRecommendedPref(const char* path);

  
  static void HandleReadError(PersistentPrefStore::PrefReadError error) {}

 protected:
  TestingPrefServiceBase(
      TestingPrefStore* managed_prefs,
      TestingPrefStore* user_prefs,
      TestingPrefStore* recommended_prefs,
      ConstructionPrefRegistry* pref_registry,
      PrefNotifierImpl* pref_notifier);

 private:
  
  
  const base::Value* GetPref(TestingPrefStore* pref_store,
                             const char* path) const;

  
  void SetPref(TestingPrefStore* pref_store, const char* path,
               base::Value* value);

  
  void RemovePref(TestingPrefStore* pref_store, const char* path);

  
  scoped_refptr<TestingPrefStore> managed_prefs_;
  scoped_refptr<TestingPrefStore> user_prefs_;
  scoped_refptr<TestingPrefStore> recommended_prefs_;

  DISALLOW_COPY_AND_ASSIGN(TestingPrefServiceBase);
};

class TestingPrefServiceSimple
    : public TestingPrefServiceBase<PrefService, PrefRegistry> {
 public:
  TestingPrefServiceSimple();
  virtual ~TestingPrefServiceSimple();

  
  
  
  
  
  PrefRegistrySimple* registry();

 private:
  DISALLOW_COPY_AND_ASSIGN(TestingPrefServiceSimple);
};

template<>
TestingPrefServiceBase<PrefService, PrefRegistry>::TestingPrefServiceBase(
    TestingPrefStore* managed_prefs,
    TestingPrefStore* user_prefs,
    TestingPrefStore* recommended_prefs,
    PrefRegistry* pref_registry,
    PrefNotifierImpl* pref_notifier);

template<class SuperPrefService, class ConstructionPrefRegistry>
TestingPrefServiceBase<
    SuperPrefService, ConstructionPrefRegistry>::~TestingPrefServiceBase() {
}

template<class SuperPrefService, class ConstructionPrefRegistry>
const base::Value* TestingPrefServiceBase<
    SuperPrefService, ConstructionPrefRegistry>::GetManagedPref(
        const char* path) const {
  return GetPref(managed_prefs_.get(), path);
}

template<class SuperPrefService, class ConstructionPrefRegistry>
void TestingPrefServiceBase<
    SuperPrefService, ConstructionPrefRegistry>::SetManagedPref(
        const char* path, base::Value* value) {
  SetPref(managed_prefs_.get(), path, value);
}

template<class SuperPrefService, class ConstructionPrefRegistry>
void TestingPrefServiceBase<
    SuperPrefService, ConstructionPrefRegistry>::RemoveManagedPref(
        const char* path) {
  RemovePref(managed_prefs_.get(), path);
}

template<class SuperPrefService, class ConstructionPrefRegistry>
const base::Value* TestingPrefServiceBase<
    SuperPrefService, ConstructionPrefRegistry>::GetUserPref(
        const char* path) const {
  return GetPref(user_prefs_.get(), path);
}

template<class SuperPrefService, class ConstructionPrefRegistry>
void TestingPrefServiceBase<
    SuperPrefService, ConstructionPrefRegistry>::SetUserPref(
        const char* path, base::Value* value) {
  SetPref(user_prefs_.get(), path, value);
}

template<class SuperPrefService, class ConstructionPrefRegistry>
void TestingPrefServiceBase<
    SuperPrefService, ConstructionPrefRegistry>::RemoveUserPref(
        const char* path) {
  RemovePref(user_prefs_.get(), path);
}

template<class SuperPrefService, class ConstructionPrefRegistry>
const base::Value* TestingPrefServiceBase<
    SuperPrefService, ConstructionPrefRegistry>::GetRecommendedPref(
        const char* path) const {
  return GetPref(recommended_prefs_, path);
}

template<class SuperPrefService, class ConstructionPrefRegistry>
void TestingPrefServiceBase<
    SuperPrefService, ConstructionPrefRegistry>::SetRecommendedPref(
        const char* path, base::Value* value) {
  SetPref(recommended_prefs_.get(), path, value);
}

template<class SuperPrefService, class ConstructionPrefRegistry>
void TestingPrefServiceBase<
    SuperPrefService, ConstructionPrefRegistry>::RemoveRecommendedPref(
        const char* path) {
  RemovePref(recommended_prefs_.get(), path);
}

template<class SuperPrefService, class ConstructionPrefRegistry>
const base::Value* TestingPrefServiceBase<
    SuperPrefService, ConstructionPrefRegistry>::GetPref(
        TestingPrefStore* pref_store, const char* path) const {
  const base::Value* res;
  return pref_store->GetValue(path, &res) ? res : NULL;
}

template<class SuperPrefService, class ConstructionPrefRegistry>
void TestingPrefServiceBase<
    SuperPrefService, ConstructionPrefRegistry>::SetPref(
        TestingPrefStore* pref_store, const char* path, base::Value* value) {
  pref_store->SetValue(path, value);
}

template<class SuperPrefService, class ConstructionPrefRegistry>
void TestingPrefServiceBase<
    SuperPrefService, ConstructionPrefRegistry>::RemovePref(
        TestingPrefStore* pref_store, const char* path) {
  pref_store->RemoveValue(path);
}

#endif  
