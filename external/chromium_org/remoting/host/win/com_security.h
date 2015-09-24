// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_WIN_COM_SECURITY_H_
#define REMOTING_HOST_WIN_COM_SECURITY_H_

#include <string>

#define SDDL_ACE(type, permissions, sid) \
    L"(" type L";;" permissions L";;;" sid L")"

#define SDDL_COM_EXECUTE_LOCAL L"0x3"

namespace remoting {

bool InitializeComSecurity(const std::string& security_descriptor,
                           const std::string& mandatory_label,
                           bool activate_as_activator);

} 

#endif  
