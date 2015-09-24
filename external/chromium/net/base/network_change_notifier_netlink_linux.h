// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_BASE_NETWORK_CHANGE_NOTIFIER_NETLINK_LINUX_H_
#define NET_BASE_NETWORK_CHANGE_NOTIFIER_NETLINK_LINUX_H_
#pragma once

#include <cstddef>

int InitializeNetlinkSocket();

bool HandleNetlinkMessage(char* buf, size_t len);

#endif  
