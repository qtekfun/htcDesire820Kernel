// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IPC_IPC_MULTIPROCESS_TEST_H_
#define IPC_IPC_MULTIPROCESS_TEST_H_

#include "testing/multiprocess_func_list.h"

#define MULTIPROCESS_IPC_TEST_MAIN(test_main) \
    MULTIPROCESS_TEST_MAIN_WITH_SETUP(test_main, \
                                      internal::MultiProcessTestIPCSetUp)

namespace internal {

void MultiProcessTestIPCSetUp();

}  

#endif  
