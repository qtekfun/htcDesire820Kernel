// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_LINUX_X11_UTIL_H_
#define REMOTING_HOST_LINUX_X11_UTIL_H_

#include <X11/Xlib.h>

#include "base/callback.h"

namespace remoting {

class ScopedXErrorHandler {
 public:
  typedef base::Callback<void(Display*, XErrorEvent*)> Handler;

  explicit ScopedXErrorHandler(const Handler& handler);
  ~ScopedXErrorHandler();

  
  
  bool ok() const { return ok_; }

  
  static Handler Ignore();

 private:
  static int HandleXErrors(Display* display, XErrorEvent* error);

  Handler handler_;
  int (*previous_handler_)(Display*, XErrorEvent*);
  bool ok_;

  DISALLOW_COPY_AND_ASSIGN(ScopedXErrorHandler);
};


class ScopedXGrabServer {
 public:
  ScopedXGrabServer(Display* display);
  ~ScopedXGrabServer();

 private:
  Display* display_;

  DISALLOW_COPY_AND_ASSIGN(ScopedXGrabServer);
};

}  

#endif  
