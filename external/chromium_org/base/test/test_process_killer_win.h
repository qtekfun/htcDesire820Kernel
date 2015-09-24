// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TEST_TEST_PROCESS_KILLER_WIN_H_
#define BASE_TEST_TEST_PROCESS_KILLER_WIN_H_

#include "base/strings/string16.h"

namespace base {

bool KillAllNamedProcessesWithArgument(const string16& process_name,
                                       const string16& argument);

}  

#endif  
