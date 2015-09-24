// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_POSIX_SIGNAL_HANDLER_H_
#define REMOTING_HOST_POSIX_SIGNAL_HANDLER_H_

#include "base/callback_forward.h"

namespace remoting {

typedef base::Callback<void(int)> SignalHandler;

bool RegisterSignalHandler(int signal_number, const SignalHandler& handler);

}  

#endif  
