// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_SHARED_IMPL_PPB_GAMEPAD_SHARED_H_
#define PPAPI_SHARED_IMPL_PPB_GAMEPAD_SHARED_H_

#include "base/atomicops.h"
#include "base/strings/string16.h"
#include "ppapi/c/ppb_gamepad.h"
#include "ppapi/shared_impl/ppapi_shared_export.h"

namespace ppapi {


#pragma pack(push, 1)

struct WebKitGamepad {
    static const size_t kIdLengthCap = 128;
    static const size_t kAxesLengthCap = 16;
    static const size_t kButtonsLengthCap = 32;

    
    bool connected;

    
    char16 id[kIdLengthCap];

    
    
    unsigned long long timestamp;

    
    unsigned axes_length;

    
    float axes[kAxesLengthCap];

    
    unsigned buttons_length;

    
    float buttons[kButtonsLengthCap];
};

struct WebKitGamepads {
    static const size_t kItemsLengthCap = 4;

    
    unsigned length;

    
    WebKitGamepad items[kItemsLengthCap];
};

struct ContentGamepadHardwareBuffer {
  base::subtle::Atomic32 sequence;
  WebKitGamepads buffer;
};

#pragma pack(pop)

PPAPI_SHARED_EXPORT void ConvertWebKitGamepadData(
    const WebKitGamepads& webkit_data,
    PP_GamepadsSampleData* output_data);

}  

#endif  
