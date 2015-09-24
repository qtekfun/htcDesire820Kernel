// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_SETUP_TEST_UTIL_H_
#define REMOTING_HOST_SETUP_TEST_UTIL_H_

#include "base/platform_file.h"

namespace remoting {

bool MakePipe(base::PlatformFile* read_handle,
              base::PlatformFile* write_handle);

}  

#endif  
