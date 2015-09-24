// Copyright 2008, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT

#ifndef GTEST_TEST_GTEST_PARAM_TEST_TEST_H_
#define GTEST_TEST_GTEST_PARAM_TEST_TEST_H_

#include "gtest/gtest.h"

#if GTEST_HAS_PARAM_TEST

class ExternalInstantiationTest : public ::testing::TestWithParam<int> {};

class InstantiationInMultipleTranslaionUnitsTest
    : public ::testing::TestWithParam<int> {};

#endif  

#endif  
