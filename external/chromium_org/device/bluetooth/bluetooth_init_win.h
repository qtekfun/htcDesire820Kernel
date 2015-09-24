// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DEVICE_BLUETOOTH_BLUETOOTH_INIT_WIN_H_
#define DEVICE_BLUETOOTH_BLUETOOTH_INIT_WIN_H_

#include <windows.h>

#include <BluetoothAPIs.h>
#if defined(_WIN32_WINNT_WIN8) && _MSC_VER < 1700
#undef FACILITY_VISUALCPP
#endif
#include <delayimp.h>
#include <ws2def.h>
#include <ws2bth.h>

#pragma comment(lib, "Bthprops.lib")

namespace device {
namespace bluetooth_init_win {

bool HasBluetoothStack();

}  
}  

#endif  
