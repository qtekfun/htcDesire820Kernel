// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_TEST_INTEGRATION_SYNC_DATATYPE_HELPER_H_
#define CHROME_BROWSER_SYNC_TEST_INTEGRATION_SYNC_DATATYPE_HELPER_H_

#include "base/basictypes.h"

class SyncTest;

namespace sync_datatype_helper {

void AssociateWithTest(SyncTest* test);

SyncTest* test();

}  

#endif  
