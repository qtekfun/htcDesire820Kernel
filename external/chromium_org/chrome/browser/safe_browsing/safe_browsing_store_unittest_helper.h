// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SAFE_BROWSING_SAFE_BROWSING_STORE_UNITTEST_HELPER_H_
#define CHROME_BROWSER_SAFE_BROWSING_SAFE_BROWSING_STORE_UNITTEST_HELPER_H_

#include "chrome/browser/safe_browsing/safe_browsing_store.h"

#include "crypto/sha2.h"
#include "testing/gtest/include/gtest/gtest.h"


inline const SBFullHash SBFullHashFromString(const char* str) {
  SBFullHash h;
  crypto::SHA256HashString(str, &h.full_hash, sizeof(h.full_hash));
  return h;
}

inline bool SBFullHashEq(const SBFullHash& a, const SBFullHash& b) {
  return !memcmp(a.full_hash, b.full_hash, sizeof(a.full_hash));
}

void SafeBrowsingStoreTestEmpty(SafeBrowsingStore* store);

void SafeBrowsingStoreTestStorePrefix(SafeBrowsingStore* store);

void SafeBrowsingStoreTestSubKnockout(SafeBrowsingStore* store);

void SafeBrowsingStoreTestDeleteChunks(SafeBrowsingStore* store);

void SafeBrowsingStoreTestDelete(SafeBrowsingStore* store,
                                 const base::FilePath& filename);

#define TEST_STORE(test_fixture, instance_name, filename)        \
  TEST_F(test_fixture, Empty) { \
    SafeBrowsingStoreTestEmpty(instance_name); \
  } \
  TEST_F(test_fixture, StorePrefix) { \
    SafeBrowsingStoreTestStorePrefix(instance_name); \
  } \
  TEST_F(test_fixture, SubKnockout) { \
    SafeBrowsingStoreTestSubKnockout(instance_name); \
  } \
  TEST_F(test_fixture, DeleteChunks) { \
    SafeBrowsingStoreTestDeleteChunks(instance_name); \
  } \
  TEST_F(test_fixture, Delete) { \
    SafeBrowsingStoreTestDelete(instance_name, filename);        \
  }

#endif  
