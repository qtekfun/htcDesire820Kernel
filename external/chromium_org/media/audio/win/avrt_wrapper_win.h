// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef MEDIA_AUDIO_WIN_AVRT_WRAPPER_WIN_H_
#define MEDIA_AUDIO_WIN_AVRT_WRAPPER_WIN_H_

#include <windows.h>
#include <avrt.h>

#include "base/basictypes.h"

namespace avrt {

bool Initialize();

bool AvRevertMmThreadCharacteristics(HANDLE avrt_handle);
HANDLE AvSetMmThreadCharacteristics(const wchar_t* task_name,
                                    DWORD* task_index);
bool AvSetMmThreadPriority(HANDLE avrt_handle, AVRT_PRIORITY priority);

}  

#endif  

