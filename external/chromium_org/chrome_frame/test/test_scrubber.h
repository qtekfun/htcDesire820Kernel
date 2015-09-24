// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_FRAME_TEST_TEST_SCRUBBER_H_
#define CHROME_FRAME_TEST_TEST_SCRUBBER_H_

#include "base/strings/string_piece.h"

namespace testing {
class UnitTest;
}

namespace chrome_frame_test {

void InstallTestScrubber(testing::UnitTest* unit_test);

void OverrideDataDirectoryForThisTest(const base::StringPiece16& user_data_dir);

}  

#endif  
