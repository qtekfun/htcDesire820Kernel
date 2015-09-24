// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_WIN_SECURITY_DESCRIPTOR_H_
#define REMOTING_HOST_WIN_SECURITY_DESCRIPTOR_H_

#include <windows.h>

#include <string>

#include "remoting/base/typed_buffer.h"

namespace remoting {

typedef TypedBuffer<ACL> ScopedAcl;
typedef TypedBuffer<SECURITY_DESCRIPTOR> ScopedSd;
typedef TypedBuffer<SID> ScopedSid;

ScopedSd ConvertSddlToSd(const std::string& sddl);

std::string ConvertSidToString(SID* sid);

ScopedSid GetLogonSid(HANDLE token);

bool MakeScopedAbsoluteSd(const ScopedSd& relative_sd,
                          ScopedSd* absolute_sd,
                          ScopedAcl* dacl,
                          ScopedSid* group,
                          ScopedSid* owner,
                          ScopedAcl* sacl);

}  

#endif  
