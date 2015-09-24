// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_ENTRY_POINT_H_
#define REMOTING_HOST_ENTRY_POINT_H_

#include "remoting/host/host_export.h"

namespace remoting {

extern const char kElevateSwitchName[];

extern const char kProcessTypeSwitchName[];

extern const char kProcessTypeController[];
extern const char kProcessTypeDaemon[];
extern const char kProcessTypeDesktop[];
extern const char kProcessTypeHost[];
extern const char kProcessTypeNativeMessagingHost[];
extern const char kProcessTypeRdpDesktopSession[];

HOST_EXPORT int HostMain(int argc, char** argv);

} 

#endif  
