// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_HWID_CHECKER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_HWID_CHECKER_H_

#include <string>

namespace chromeos {

bool IsHWIDCorrect(const std::string& hwid);

bool IsMachineHWIDCorrect();

} 

#endif  

