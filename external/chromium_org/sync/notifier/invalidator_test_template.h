// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef SYNC_NOTIFIER_INVALIDATOR_TEST_TEMPLATE_H_
#define SYNC_NOTIFIER_INVALIDATOR_TEST_TEMPLATE_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "google/cacheinvalidation/include/types.h"
#include "google/cacheinvalidation/types.pb.h"
#include "sync/internal_api/public/base/object_id_invalidation_map_test_util.h"
#include "sync/notifier/fake_invalidation_handler.h"
#include "sync/notifier/fake_invalidation_state_tracker.h"
#include "sync/notifier/invalidator.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace syncer {

template <typename InvalidatorTestDelegate>
class InvalidatorTest : public testing::Test {
 protected:
  InvalidatorTest()
      : id1(ipc::invalidation::ObjectSource::TEST, "a"),
        id2(ipc::invalidation::ObjectSource::TEST, "b"),
        id3(ipc::invalidation::ObjectSource::TEST, "c"),
        id4(ipc::invalidation::ObjectSource::TEST, "d") {
  }

  Invalidator* CreateAndInitializeInvalidator() {
    this->delegate_.CreateInvalidator("fake_invalidator_client_id",
                                      "fake_initial_state",
                                      this->fake_tracker_.AsWeakPtr());
    Invalidator* const invalidator = this->delegate_.GetInvalidator();

    this->delegate_.WaitForInvalidator();
    invalidator->UpdateCredentials("foo@bar.com", "fake_token");
    this->delegate_.WaitForInvalidator();

    return invalidator;
  }

  FakeInvalidationStateTracker fake_tracker_;
  InvalidatorTestDelegate delegate_;

  const invalidation::ObjectId id1;
  const invalidation::ObjectId id2;
  const invalidation::ObjectId id3;
  const invalidation::ObjectId id4;
};

TYPED_TEST_CASE_P(InvalidatorTest);

TYPED_TEST_P(InvalidatorTest, Basic) {
  Invalidator* const invalidator = this->CreateAndInitializeInvalidator();

  FakeInvalidationHandler handler;

  invalidator->RegisterHandler(&handler);

  ObjectIdInvalidationMap invalidation_map;
  invalidation_map.Insert(Invalidation::Init(this->id1, 1, "1"));
  invalidation_map.Insert(Invalidation::Init(this->id2, 2, "2"));
  invalidation_map.Insert(Invalidation::Init(this->id3, 3, "3"));

  
  this->delegate_.TriggerOnIncomingInvalidation(invalidation_map);
  EXPECT_EQ(0, handler.GetInvalidationCount());

  ObjectIdSet ids;
  ids.insert(this->id1);
  ids.insert(this->id2);
  invalidator->UpdateRegisteredIds(&handler, ids);

  this->delegate_.TriggerOnInvalidatorStateChange(INVALIDATIONS_ENABLED);
  EXPECT_EQ(INVALIDATIONS_ENABLED, handler.GetInvalidatorState());

  ObjectIdInvalidationMap expected_invalidations;
  expected_invalidations.Insert(Invalidation::Init(this->id1, 1, "1"));
  expected_invalidations.Insert(Invalidation::Init(this->id2, 2, "2"));

  this->delegate_.TriggerOnIncomingInvalidation(invalidation_map);
  EXPECT_EQ(1, handler.GetInvalidationCount());
  EXPECT_THAT(expected_invalidations, Eq(handler.GetLastInvalidationMap()));

  ids.erase(this->id1);
  ids.insert(this->id3);
  invalidator->UpdateRegisteredIds(&handler, ids);

  expected_invalidations = ObjectIdInvalidationMap();
  expected_invalidations.Insert(Invalidation::Init(this->id2, 2, "2"));
  expected_invalidations.Insert(Invalidation::Init(this->id3, 3, "3"));

  
  this->delegate_.TriggerOnIncomingInvalidation(invalidation_map);
  EXPECT_EQ(2, handler.GetInvalidationCount());
  EXPECT_THAT(expected_invalidations, Eq(handler.GetLastInvalidationMap()));

  this->delegate_.TriggerOnInvalidatorStateChange(TRANSIENT_INVALIDATION_ERROR);
  EXPECT_EQ(TRANSIENT_INVALIDATION_ERROR,
            handler.GetInvalidatorState());

  this->delegate_.TriggerOnInvalidatorStateChange(
      INVALIDATION_CREDENTIALS_REJECTED);
  EXPECT_EQ(INVALIDATION_CREDENTIALS_REJECTED,
            handler.GetInvalidatorState());

  invalidator->UnregisterHandler(&handler);

  
  this->delegate_.TriggerOnIncomingInvalidation(invalidation_map);
  EXPECT_EQ(2, handler.GetInvalidationCount());
}

TYPED_TEST_P(InvalidatorTest, MultipleHandlers) {
  Invalidator* const invalidator = this->CreateAndInitializeInvalidator();

  FakeInvalidationHandler handler1;
  FakeInvalidationHandler handler2;
  FakeInvalidationHandler handler3;
  FakeInvalidationHandler handler4;

  invalidator->RegisterHandler(&handler1);
  invalidator->RegisterHandler(&handler2);
  invalidator->RegisterHandler(&handler3);
  invalidator->RegisterHandler(&handler4);

  {
    ObjectIdSet ids;
    ids.insert(this->id1);
    ids.insert(this->id2);
    invalidator->UpdateRegisteredIds(&handler1, ids);
  }

  {
    ObjectIdSet ids;
    ids.insert(this->id3);
    invalidator->UpdateRegisteredIds(&handler2, ids);
  }

  

  {
    ObjectIdSet ids;
    ids.insert(this->id4);
    invalidator->UpdateRegisteredIds(&handler4, ids);
  }

  invalidator->UnregisterHandler(&handler4);

  this->delegate_.TriggerOnInvalidatorStateChange(INVALIDATIONS_ENABLED);
  EXPECT_EQ(INVALIDATIONS_ENABLED, handler1.GetInvalidatorState());
  EXPECT_EQ(INVALIDATIONS_ENABLED, handler2.GetInvalidatorState());
  EXPECT_EQ(INVALIDATIONS_ENABLED, handler3.GetInvalidatorState());
  EXPECT_EQ(TRANSIENT_INVALIDATION_ERROR, handler4.GetInvalidatorState());

  {
    ObjectIdInvalidationMap invalidation_map;
    invalidation_map.Insert(Invalidation::Init(this->id1, 1, "1"));
    invalidation_map.Insert(Invalidation::Init(this->id2, 2, "2"));
    invalidation_map.Insert(Invalidation::Init(this->id3, 3, "3"));
    invalidation_map.Insert(Invalidation::Init(this->id4, 4, "4"));

    this->delegate_.TriggerOnIncomingInvalidation(invalidation_map);

    ObjectIdInvalidationMap expected_invalidations;
    expected_invalidations.Insert(Invalidation::Init(this->id1, 1, "1"));
    expected_invalidations.Insert(Invalidation::Init(this->id2, 2, "2"));

    EXPECT_EQ(1, handler1.GetInvalidationCount());
    EXPECT_THAT(expected_invalidations, Eq(handler1.GetLastInvalidationMap()));

    expected_invalidations = ObjectIdInvalidationMap();
    expected_invalidations.Insert(Invalidation::Init(this->id3, 3, "3"));

    EXPECT_EQ(1, handler2.GetInvalidationCount());
    EXPECT_THAT(expected_invalidations, Eq(handler2.GetLastInvalidationMap()));

    EXPECT_EQ(0, handler3.GetInvalidationCount());
    EXPECT_EQ(0, handler4.GetInvalidationCount());
  }

  this->delegate_.TriggerOnInvalidatorStateChange(TRANSIENT_INVALIDATION_ERROR);
  EXPECT_EQ(TRANSIENT_INVALIDATION_ERROR, handler1.GetInvalidatorState());
  EXPECT_EQ(TRANSIENT_INVALIDATION_ERROR, handler2.GetInvalidatorState());
  EXPECT_EQ(TRANSIENT_INVALIDATION_ERROR, handler3.GetInvalidatorState());
  EXPECT_EQ(TRANSIENT_INVALIDATION_ERROR, handler4.GetInvalidatorState());

  invalidator->UnregisterHandler(&handler3);
  invalidator->UnregisterHandler(&handler2);
  invalidator->UnregisterHandler(&handler1);
}

TYPED_TEST_P(InvalidatorTest, EmptySetUnregisters) {
  Invalidator* const invalidator = this->CreateAndInitializeInvalidator();

  FakeInvalidationHandler handler1;

  
  FakeInvalidationHandler handler2;

  invalidator->RegisterHandler(&handler1);
  invalidator->RegisterHandler(&handler2);

  {
    ObjectIdSet ids;
    ids.insert(this->id1);
    ids.insert(this->id2);
    invalidator->UpdateRegisteredIds(&handler1, ids);
  }

  {
    ObjectIdSet ids;
    ids.insert(this->id3);
    invalidator->UpdateRegisteredIds(&handler2, ids);
  }

  
  
  invalidator->UpdateRegisteredIds(&handler1, ObjectIdSet());

  this->delegate_.TriggerOnInvalidatorStateChange(INVALIDATIONS_ENABLED);
  EXPECT_EQ(INVALIDATIONS_ENABLED, handler1.GetInvalidatorState());
  EXPECT_EQ(INVALIDATIONS_ENABLED, handler2.GetInvalidatorState());

  {
    ObjectIdInvalidationMap invalidation_map;
    invalidation_map.Insert(Invalidation::Init(this->id1, 1, "1"));
    invalidation_map.Insert(Invalidation::Init(this->id2, 2, "2"));
    invalidation_map.Insert(Invalidation::Init(this->id3, 3, "3"));
    this->delegate_.TriggerOnIncomingInvalidation(invalidation_map);
    EXPECT_EQ(0, handler1.GetInvalidationCount());
    EXPECT_EQ(1, handler2.GetInvalidationCount());
  }

  this->delegate_.TriggerOnInvalidatorStateChange(TRANSIENT_INVALIDATION_ERROR);
  EXPECT_EQ(TRANSIENT_INVALIDATION_ERROR, handler1.GetInvalidatorState());
  EXPECT_EQ(TRANSIENT_INVALIDATION_ERROR, handler2.GetInvalidatorState());

  invalidator->UnregisterHandler(&handler2);
  invalidator->UnregisterHandler(&handler1);
}

namespace internal {

class BoundFakeInvalidationHandler : public FakeInvalidationHandler {
 public:
  explicit BoundFakeInvalidationHandler(const Invalidator& invalidator);
  virtual ~BoundFakeInvalidationHandler();

  
  
  
  InvalidatorState GetLastRetrievedState() const;

  
  virtual void OnInvalidatorStateChange(InvalidatorState state) OVERRIDE;

 private:
  const Invalidator& invalidator_;
  InvalidatorState last_retrieved_state_;

  DISALLOW_COPY_AND_ASSIGN(BoundFakeInvalidationHandler);
};

}  

TYPED_TEST_P(InvalidatorTest, GetInvalidatorStateAlwaysCurrent) {
  Invalidator* const invalidator = this->CreateAndInitializeInvalidator();

  internal::BoundFakeInvalidationHandler handler(*invalidator);
  invalidator->RegisterHandler(&handler);

  this->delegate_.TriggerOnInvalidatorStateChange(INVALIDATIONS_ENABLED);
  EXPECT_EQ(INVALIDATIONS_ENABLED, handler.GetInvalidatorState());
  EXPECT_EQ(INVALIDATIONS_ENABLED, handler.GetLastRetrievedState());

  this->delegate_.TriggerOnInvalidatorStateChange(TRANSIENT_INVALIDATION_ERROR);
  EXPECT_EQ(TRANSIENT_INVALIDATION_ERROR, handler.GetInvalidatorState());
  EXPECT_EQ(TRANSIENT_INVALIDATION_ERROR, handler.GetLastRetrievedState());

  invalidator->UnregisterHandler(&handler);
}

REGISTER_TYPED_TEST_CASE_P(InvalidatorTest,
                           Basic, MultipleHandlers, EmptySetUnregisters,
                           GetInvalidatorStateAlwaysCurrent);

}  

#endif  
