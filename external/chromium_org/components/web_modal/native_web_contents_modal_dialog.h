// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_WEB_MODAL_NATIVE_WEB_CONTENTS_MODAL_DIALOG_H_
#define COMPONENTS_WEB_MODAL_NATIVE_WEB_CONTENTS_MODAL_DIALOG_H_

#include "ui/gfx/native_widget_types.h"

namespace web_modal {

#if defined(OS_MACOSX)
typedef void* NativeWebContentsModalDialog;
#else
typedef gfx::NativeView NativeWebContentsModalDialog;
#endif

}  

#endif  
