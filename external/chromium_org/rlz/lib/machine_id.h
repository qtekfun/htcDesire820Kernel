// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RLZ_LIB_MACHINE_ID_H_
#define RLZ_LIB_MACHINE_ID_H_

#include "base/strings/string16.h"

#include <string>

namespace rlz_lib {

bool GetMachineId(std::string* machine_id);

bool GetRawMachineId(string16* data, int* more_data);

namespace testing {
bool GetMachineIdImpl(const string16& sid_string,
                      int volume_id,
                      std::string* machine_id);
}  

}  

#endif  
