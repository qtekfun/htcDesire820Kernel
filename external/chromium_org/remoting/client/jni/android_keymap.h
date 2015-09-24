// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_CLIENT_JNI_ANDROID_KEYMAP_H_
#define REMOTING_CLIENT_JNI_ANDROID_KEYMAP_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"

namespace remoting {

uint32 AndroidKeycodeToUsbKeycode(size_t android);

}  

#endif
