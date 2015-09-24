// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_TEST_LOGGING_WIN_TEST_LOG_COLLECTOR_H_
#define CHROME_TEST_LOGGING_WIN_TEST_LOG_COLLECTOR_H_

namespace testing {
class UnitTest;
}

namespace logging_win {

void InstallTestLogCollector(testing::UnitTest* unit_test);

}  

#endif  
