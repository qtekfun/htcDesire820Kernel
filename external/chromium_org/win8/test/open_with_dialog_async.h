// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WIN8_TEST_OPEN_WITH_DIALOG_ASYNC_H_
#define WIN8_TEST_OPEN_WITH_DIALOG_ASYNC_H_

#include <windows.h>

#include "base/callback_forward.h"
#include "base/strings/string16.h"

namespace win8 {

typedef base::Callback<void(HRESULT)> OpenWithDialogCallback;

void OpenWithDialogAsync(HWND parent_window,
                         const string16& file_name,
                         const string16& file_type_class,
                         int open_as_info_flags,
                         const OpenWithDialogCallback& callback);

}  

#endif  
