// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_INVALIDATION_INVALIDATION_SERVICE_TEST_TEMPLATE_H_
#define CHROME_BROWSER_INVALIDATION_INVALIDATION_SERVICE_TEST_TEMPLATE_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/invalidation/invalidation_service.h"
#include "google/cacheinvalidation/include/types.h"
#include "google/cacheinvalidation/types.pb.h"
#include "sync/internal_api/public/base/ack_handle.h"
#include "sync/internal_api/public/base/invalidation.h"
#include "sync/internal_api/public/base/object_id_invalidation_map_test_util.h"
#include "sync/notifier/fake_invalidation_handler.h"
#include "sync/notifier/object_id_invalidation_map.h"
#include "testing/gtest/include/gtest/gtest.h"

template <typename InvalidatorTestDelegate>
class InvalidationServiceTest : public testing::Test {
 protected:
  InvalidationServiceTest()
      : id1(ipc::invalidation::ObjectSource::CHROME_SYNC, "BOOKMARK"),
        id2(ipc::invalidation::ObjectSource::CHROME_SYNC, "PREFERENCE"),
        id3(ipc::invalidation::ObjectSource::CHROME_SYNC, "AUTOFILL"),
        id4(ipc::invalidation::ObjectSource::CHROME_PUSH_MESSAGING,
            "PUSH_MESSAGE") {
  }

  invalidation::InvalidationService*
  CreateAndInitializeInvalidationService() {
    this->delegate_.CreateInvalidationService();
    return this->delegate_.GetInvalidationService();
  }

  InvalidatorTestDelegate delegate_;

  const invalidation::ObjectId id1;
  const invalidation::ObjectId id2;
  const invalidation::ObjectId id3;
  const invalidation::ObjectId id4;
};

TYPED_TEST_CASE_P(InvalidationServiceTest);

TYPED_TEST_P(InvalidationServiceTest, Basic) {
  invalidation::InvalidationService* const invalidator =
      this->CreateAndInitializeInvalidationService();

  syncer::FakeInvalidationHandler handler;

  invalidator->RegisterInvalidationHandler(&handler);

  syncer::ObjectIdInvalidationMap invalidation_map;
  invalidation_map.Insert(syncer::Invalidation::Init(this->id1, 1, "1"));
  invalidation_map.Insert(syncer::Invalidation::Init(this->id2, 2, "2"));
  invalidation_map.Insert(syncer::Invalidation::Init(this->id3, 3, "3"));

  
  this->delegate_.TriggerOnIncomingInvalidation(invalidation_map);
  EXPECT_EQ(0, handler.GetInvalidationCount());

  syncer::ObjectIdSet ids;
  ids.insert(this->id1);
  ids.insert(this->id2);
  invalidator->UpdateRegisteredInvalidationIds(&handler, ids);

  this->delegate_.TriggerOnInvalidatorStateChange(
      syncer::INVALIDATIONS_ENABLED);
  EXPECT_EQ(syncer::INVALIDATIONS_ENABLED, handler.GetInvalidatorState());

  syncer::ObjectIdInvalidationMap expected_invalidations;
  expected_invalidations.Insert(syncer::Invalidation::Init(this->id1, 1, "1"));
  expected_invalidations.Insert(syncer::Invalidation::Init(this->id2, 2, "2"));

  this->delegate_.TriggerOnIncomingInvalidation(invalidation_map);
  EXPECT_EQ(1, handler.GetInvalidationCount());
  EXPECT_THAT(expected_invalidations, Eq(handler.GetLastInvalidationMap()));

  ids.erase(this->id1);
  ids.insert(this->id3);
  invalidator->UpdateRegisteredInvalidationIds(&handler, ids);

  expected_invalidations = syncer::ObjectIdInvalidationMap();
  expected_invalidations.Insert(syncer::Invalidation::Init(this->id2, 2, "2"));
  expected_invalidations.Insert(syncer::Invalidation::Init(this->id3, 3, "3"));

  
  this->delegate_.TriggerOnIncomingInvalidation(invalidation_map);
  EXPECT_EQ(2, handler.GetInvalidationCount());
  EXPECT_THAT(expected_invalidations, Eq(handler.GetLastInvalidationMap()));

  this->delegate_.TriggerOnInvalidatorStateChange(
      syncer::TRANSIENT_INVALIDATION_ERROR);
  EXPECT_EQ(syncer::TRANSIENT_INVALIDATION_ERROR,
            handler.GetInvalidatorState());

  this->delegate_.TriggerOnInvalidatorStateChange(
      syncer::INVALIDATIONS_ENABLED);
  EXPECT_EQ(syncer::INVALIDATIONS_ENABLED,
            handler.GetInvalidatorState());

  invalidator->UnregisterInvalidationHandler(&handler);

  
  this->delegate_.TriggerOnIncomingInvalidation(invalidation_map);
  EXPECT_EQ(2, handler.GetInvalidationCount());
}

TYPED_TEST_P(InvalidationServiceTest, MultipleHandlers) {
  invalidation::InvalidationService* const invalidator =
      this->CreateAndInitializeInvalidationService();

  syncer::FakeInvalidationHandler handler1;
  syncer::FakeInvalidationHandler handler2;
  syncer::FakeInvalidationHandler handler3;
  syncer::FakeInvalidationHandler handler4;

  invalidator->RegisterInvalidationHandler(&handler1);
  invalidator->RegisterInvalidationHandler(&handler2);
  invalidator->RegisterInvalidationHandler(&handler3);
  invalidator->RegisterInvalidationHandler(&handler4);

  {
    syncer::ObjectIdSet ids;
    ids.insert(this->id1);
    ids.insert(this->id2);
    invalidator->UpdateRegisteredInvalidationIds(&handler1, ids);
  }

  {
    syncer::ObjectIdSet ids;
    ids.insert(this->id3);
    invalidator->UpdateRegisteredInvalidationIds(&handler2, ids);
  }

  

  {
    syncer::ObjectIdSet ids;
    ids.insert(this->id4);
    invalidator->UpdateRegisteredInvalidationIds(&handler4, ids);
  }

  invalidator->UnregisterInvalidationHandler(&handler4);

  this->delegate_.TriggerOnInvalidatorStateChange(
      syncer::INVALIDATIONS_ENABLED);
  EXPECT_EQ(syncer::INVALIDATIONS_ENABLED, handler1.GetInvalidatorState());
  EXPECT_EQ(syncer::INVALIDATIONS_ENABLED, handler2.GetInvalidatorState());
  EXPECT_EQ(syncer::INVALIDATIONS_ENABLED, handler3.GetInvalidatorState());
  EXPECT_EQ(syncer::TRANSIENT_INVALIDATION_ERROR,
            handler4.GetInvalidatorState());

  {
    syncer::ObjectIdInvalidationMap invalidation_map;
    invalidation_map.Insert(syncer::Invalidation::Init(this->id1, 1, "1"));
    invalidation_map.Insert(syncer::Invalidation::Init(this->id2, 2, "2"));
    invalidation_map.Insert(syncer::Invalidation::Init(this->id3, 3, "3"));
    invalidation_map.Insert(syncer::Invalidation::Init(this->id4, 4, "4"));
    this->delegate_.TriggerOnIncomingInvalidation(invalidation_map);

    syncer::ObjectIdInvalidationMap expected_invalidations;
    expected_invalidations.Insert(
        syncer::Invalidation::Init(this->id1, 1, "1"));
    expected_invalidations.Insert(
        syncer::Invalidation::Init(this->id2, 2, "2"));

    EXPECT_EQ(1, handler1.GetInvalidationCount());
    EXPECT_THAT(expected_invalidations, Eq(handler1.GetLastInvalidationMap()));

    expected_invalidations = syncer::ObjectIdInvalidationMap();
    expected_invalidations.Insert(
        syncer::Invalidation::Init(this->id3, 3, "3"));

    EXPECT_EQ(1, handler2.GetInvalidationCount());
    EXPECT_THAT(expected_invalidations, Eq(handler2.GetLastInvalidationMap()));

    EXPECT_EQ(0, handler3.GetInvalidationCount());
    EXPECT_EQ(0, handler4.GetInvalidationCount());
  }

  this->delegate_.TriggerOnInvalidatorStateChange(
      syncer::TRANSIENT_INVALIDATION_ERROR);
  EXPECT_EQ(syncer::TRANSIENT_INVALIDATION_ERROR,
            handler1.GetInvalidatorState());
  EXPECT_EQ(syncer::TRANSIENT_INVALIDATION_ERROR,
            handler2.GetInvalidatorState());
  EXPECT_EQ(syncer::TRANSIENT_INVALIDATION_ERROR,
            handler3.GetInvalidatorState());
  EXPECT_EQ(syncer::TRANSIENT_INVALIDATION_ERROR,
            handler4.GetInvalidatorState());

  invalidator->UnregisterInvalidationHandler(&handler3);
  invalidator->UnregisterInvalidationHandler(&handler2);
  invalidator->UnregisterInvalidationHandler(&handler1);
}

TYPED_TEST_P(InvalidationServiceTest, EmptySetUnregisters) {
  invalidation::InvalidationService* const invalidator =
      this->CreateAndInitializeInvalidationService();

  syncer::FakeInvalidationHandler handler1;

  
  syncer::FakeInvalidationHandler handler2;

  invalidator->RegisterInvalidationHandler(&handler1);
  invalidator->RegisterInvalidationHandler(&handler2);

  {
    syncer::ObjectIdSet ids;
    ids.insert(this->id1);
    ids.insert(this->id2);
    invalidator->UpdateRegisteredInvalidationIds(&handler1, ids);
  }

  {
    syncer::ObjectIdSet ids;
    ids.insert(this->id3);
    invalidator->UpdateRegisteredInvalidationIds(&handler2, ids);
  }

  
  
  invalidator->UpdateRegisteredInvalidationIds(&handler1,
                                               syncer::ObjectIdSet());

  this->delegate_.TriggerOnInvalidatorStateChange(
      syncer::INVALIDATIONS_ENABLED);
  EXPECT_EQ(syncer::INVALIDATIONS_ENABLED, handler1.GetInvalidatorState());
  EXPECT_EQ(syncer::INVALIDATIONS_ENABLED, handler2.GetInvalidatorState());

  {
    syncer::ObjectIdInvalidationMap invalidation_map;
    invalidation_map.Insert(syncer::Invalidation::Init(this->id1, 1, "1"));
    invalidation_map.Insert(syncer::Invalidation::Init(this->id2, 2, "2"));
    invalidation_map.Insert(syncer::Invalidation::Init(this->id3, 3, "3"));
    this->delegate_.TriggerOnIncomingInvalidation(invalidation_map);
    EXPECT_EQ(0, handler1.GetInvalidationCount());
    EXPECT_EQ(1, handler2.GetInvalidationCount());
  }

  this->delegate_.TriggerOnInvalidatorStateChange(
      syncer::TRANSIENT_INVALIDATION_ERROR);
  EXPECT_EQ(syncer::TRANSIENT_INVALIDATION_ERROR,
            handler1.GetInvalidatorState());
  EXPECT_EQ(syncer::TRANSIENT_INVALIDATION_ERROR,
            handler2.GetInvalidatorState());

  invalidator->UnregisterInvalidationHandler(&handler2);
  invalidator->UnregisterInvalidationHandler(&handler1);
}

namespace internal {

class BoundFakeInvalidationHandler : public syncer::FakeInvalidationHandler {
 public:
  explicit BoundFakeInvalidationHandler(
      const invalidation::InvalidationService& invalidator);
  virtual ~BoundFakeInvalidationHandler();

  
  
  
  syncer::InvalidatorState GetLastRetrievedState() const;

  
  virtual void OnInvalidatorStateChange(
      syncer::InvalidatorState state) OVERRIDE;

 private:
  const invalidation::InvalidationService& invalidator_;
  syncer::InvalidatorState last_retrieved_state_;

  DISALLOW_COPY_AND_ASSIGN(BoundFakeInvalidationHandler);
};

}  

TYPED_TEST_P(InvalidationServiceTest, GetInvalidatorStateAlwaysCurrent) {
  invalidation::InvalidationService* const invalidator =
      this->CreateAndInitializeInvalidationService();

  internal::BoundFakeInvalidationHandler handler(*invalidator);
  invalidator->RegisterInvalidationHandler(&handler);

  this->delegate_.TriggerOnInvalidatorStateChange(
      syncer::INVALIDATIONS_ENABLED);
  EXPECT_EQ(syncer::INVALIDATIONS_ENABLED, handler.GetInvalidatorState());
  EXPECT_EQ(syncer::INVALIDATIONS_ENABLED, handler.GetLastRetrievedState());

  this->delegate_.TriggerOnInvalidatorStateChange(
      syncer::TRANSIENT_INVALIDATION_ERROR);
  EXPECT_EQ(syncer::TRANSIENT_INVALIDATION_ERROR,
            handler.GetInvalidatorState());
  EXPECT_EQ(syncer::TRANSIENT_INVALIDATION_ERROR,
            handler.GetLastRetrievedState());

  invalidator->UnregisterInvalidationHandler(&handler);
}

REGISTER_TYPED_TEST_CASE_P(InvalidationServiceTest,
                           Basic, MultipleHandlers, EmptySetUnregisters,
                           GetInvalidatorStateAlwaysCurrent);

#endif  
