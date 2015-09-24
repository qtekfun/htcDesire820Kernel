// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SANDBOX_SRC_SID_H_
#define SANDBOX_SRC_SID_H_

#include <windows.h>

namespace sandbox {

class Sid {
 public:
  
  
  Sid(const SID *sid);
  Sid(WELL_KNOWN_SID_TYPE type);

  
  const SID *GetPSID() const;

 private:
  BYTE sid_[SECURITY_MAX_SID_SIZE];
};

}  

#endif  
