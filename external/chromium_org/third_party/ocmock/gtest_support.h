// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_OCMOCK_GTEST_SUPPORT_H_
#define THIRD_PARTY_OCMOCK_GTEST_SUPPORT_H_

#include "testing/gtest/include/gtest/gtest.h"

@class OCMockObject;

namespace testing {
namespace internal {
bool VerifyOCMock(OCMockObject* mock, const char* file, int line);
}  
}  

#define EXPECT_OCMOCK_VERIFY(m) \
    EXPECT_TRUE(testing::internal::VerifyOCMock((m), __FILE__, __LINE__))
#define ASSERT_OCMOCK_VERIFY(m) \
    ASSERT_TRUE(testing::internal::VerifyOCMock((m), __FILE__, __LINE__))

#endif  
